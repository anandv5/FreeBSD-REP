/*-
 * Copyright (c) 2014 Danilo Egea Gondolfo <danilo@FreeBSD.org>
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR AND CONTRIBUTORS ``AS IS'' AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE AUTHOR OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 */


#include <sys/cdefs.h>
__FBSDID("$FreeBSD:$");

#include <sys/param.h>
#include <sys/kernel.h>
#include <sys/libkern.h>
#include <sys/socket.h>
#include <sys/socketvar.h>
#include <sys/sysctl.h>
#include <sys/systm.h>
#include <sys/mbuf.h>
#include <sys/types.h>
#include <sys/counter.h>
#include <sys/hash.h>
#include <sys/protosw.h>
#include <sys/domain.h>


#include <net/if.h>
#include <net/if_var.h>
#include <net/if_dl.h>
#include <net/ethernet.h>
#include <net/netisr.h>


#include <netrepi/repi.h>
#include <netrepi/repi_pcb.h>

/* Hash used to identify a packet already forwarded */
static int *repi_hash = NULL;

/* Statistics */

struct repi_stats repi_stats;

/* sysctls functions handlers */

struct ifnet *repi_ifnet = NULL;

int repi_bind_hash_size = 4096;

static int sysctl_repi_input_dgrams_count_handler(SYSCTL_HANDLER_ARGS) {

	int error;
	uint64_t val;

	val = counter_u64_fetch(repi_stats.repi_input_dgrams_count);
	error = SYSCTL_OUT(req, &val, sizeof(val));

	return error;

}

static int sysctl_repi_output_dgrams_count_handler(SYSCTL_HANDLER_ARGS) {

	int error;
	uint64_t val;

	val = counter_u64_fetch(repi_stats.repi_output_dgrams_count);
	error = SYSCTL_OUT(req, &val, sizeof(val));

	return error;

}

static int sysctl_repi_forwarded_dgrams_count_handler(SYSCTL_HANDLER_ARGS) {

	int error;
	uint64_t val;

	val = counter_u64_fetch(repi_stats.repi_forwarded_dgrams_count);
	error = SYSCTL_OUT(req, &val, sizeof(val));

	return error;

}

SYSCTL_NODE(_net, OID_AUTO, repi, CTLFLAG_RD, 0, "REPI Protocol");

SYSCTL_NODE(_net_repi, OID_AUTO, stats, CTLFLAG_RD, 0, "REPI Statistics");
SYSCTL_PROC(_net_repi_stats, OID_AUTO, input_packets, CTLTYPE_ULONG|CTLFLAG_RD, NULL, 0,
		sysctl_repi_input_dgrams_count_handler, "LU", "Number of input packets");
SYSCTL_PROC(_net_repi_stats, OID_AUTO, output_packets, CTLTYPE_ULONG|CTLFLAG_RD, NULL, 0,
		sysctl_repi_output_dgrams_count_handler, "LU", "Number of output packets");
SYSCTL_PROC(_net_repi_stats, OID_AUTO, forwarded_packets, CTLTYPE_ULONG|CTLFLAG_RD, NULL, 0,
		sysctl_repi_forwarded_dgrams_count_handler, "LU", "Number of forwarded packets");

SYSCTL_NODE(_net_repi, OID_AUTO, control, CTLFLAG_RD, 0, "REPI Control");
SYSCTL_INT(_net_repi_control, OID_AUTO, random_mac_address, CTLFLAG_RW, &repi_random_mac_address, 0,
		"Generate random MAC address for packets");
SYSCTL_INT(_net_repi_control, OID_AUTO, disable_forwarding, CTLFLAG_RW, &repi_packets_forwarding_disabled, 0,
		"Disable packets forwarding");
SYSCTL_INT(_net_repi_control, OID_AUTO, hash_size, CTLFLAG_RDTUN, &repi_hash_size, 0,
		"Packet identifier hash size");

static void
repi_randomize_mac_address(u_char *mac) {

	int i;

	for(i = 0; i < ETHER_ADDR_LEN; i++)
		mac[i] = arc4random() % 256;

}

int
repi_output(struct ifnet *ifp, struct mbuf *m) {

	struct sockaddr addr;
	struct ether_header *eh;
	int error = 0;

	/* repi_ifnet is asigned by the ioctl command 
	 * and is the global and uniq interface used by repi
	 */
	if(repi_ifnet == NULL) {
		printf("nullo\n");
		return -1;
	}

	printf("iface %s\n", repi_ifnet->if_xname);

	addr.sa_family = AF_REPI;
	addr.sa_len = m->m_hdr.mh_len;

	eh = (struct ether_header *) &addr.sa_data;
	eh->ether_type = htons(ETHERTYPE_REPI);
	memcpy(eh->ether_dhost, repi_ifnet->if_broadcastaddr, ETHER_ADDR_LEN);

	/* Randomize MAC address? */
	if(repi_random_mac_address)
		repi_randomize_mac_address(eh->ether_shost);
	else
		memcpy(eh->ether_shost, IF_LLADDR(repi_ifnet), ETHER_ADDR_LEN);

	/* Update statistics */
	counter_u64_add(repi_stats.repi_output_dgrams_count, 1);

	/* Fly packet, fly! */
	error = (*repi_ifnet->if_output)(repi_ifnet, m, &addr, NULL);

	m_freem(m);

	return error;

}

static int
repi_forward(struct ifnet *ifp, struct mbuf *m) {

	struct sockaddr addr;
	struct ether_header *eh;

	addr.sa_family = AF_REPI;
	addr.sa_len = m->m_hdr.mh_len;

	eh = (struct ether_header *) &addr.sa_data;
	eh->ether_type = htons(ETHERTYPE_REPI);
	memcpy(eh->ether_dhost, ifp->if_broadcastaddr, ETHER_ADDR_LEN);

	/* Update statistics */
	counter_u64_add(repi_stats.repi_forwarded_dgrams_count, 1);
	counter_u64_add(repi_stats.repi_output_dgrams_count, 1);

	/* Fly packet, fly! */
	return((*ifp->if_output)(ifp, m, &addr, NULL));
}

static void
repi_input_internal(struct mbuf *m) {

	struct repi_header *repi_hdr;
	struct ifnet *ifp = m->m_pkthdr.rcvif;
	struct repi_pcb *pcb;

	uint32_t hash;

	printf("Packet input...\n");
	printf("Size: %d\n", m->m_hdr.mh_len);

	repi_hdr = (struct repi_header *) m->m_hdr.mh_data;

	printf("Version: %d\n", repi_hdr->version);
	printf("Flags: %x%x%x\n", repi_hdr->hide_flag, repi_hdr->crypto, repi_hdr->other_flags);
	printf("TTL: %d\n", repi_hdr->ttl);
	printf("hlen: %d\n", repi_hdr->hlen);
	printf("seqnumber: %d\n", repi_hdr->seq_number);
	printf("dst prefix: %x\n", repi_hdr->prefix_dst);
	printf("src prefix: %x\n", repi_hdr->prefix_src);
	printf("timestamp: %llu\n", repi_hdr->timestamp);
	printf("interest: %u\n", repi_hdr->interest);

	/* TODO: Implementar o recvfrom aqui */

	/* Reply the packet to the other machines? */
	if(!repi_packets_forwarding_disabled) {
		/* TODO: Implementar as restricoes para o forwarding */

		/* Decrement TTL */
		repi_hdr->ttl--;

		/* If the packet TTL reach zero, don't send to network */
		if(repi_hdr->ttl == 0) goto zero_ttl;

		hash = REPI_HASH_PACKET_CREATE(repi_hdr);

		/* If the packet already passed over here, don't send to network again */
		if(repi_hash[REPI_HASH_MOD(hash)] != -1) goto packet_already_forwarded;

		/* TODO: Implementar uma maneira de remover os numeros de sequencia do hash apos algum tempo.
		 *	talvez atraves de uma fila circular com um tamanho razoavel
		 * */

		/* Add the packet to hash table */
		repi_hash[REPI_HASH_MOD(hash)] = repi_hdr->seq_number;

		repi_forward(ifp, m);
	}

	/* TODO: O procedimento para acordar o processo que esta travado no recvfrom depende do bind estar funcionando 
	 * Para isso o segundo cabe�alho precisa estar funcionando
	 */
	pcb = repi_pcb_get_by_hash(*((uint32_t *)mtod(m, struct repi_header *) + sizeof(struct repi_header)));
	/* TODO: nao esta funcionando */
	if(pcb->so)
		sorwakeup(pcb->so);

zero_ttl:
packet_already_forwarded:

	m_free(m);

	/* Update statistics */
	counter_u64_add(repi_stats.repi_input_dgrams_count, 1);

}

static void
repi_nh_input(struct mbuf *m) {

	repi_input_internal(m);

}

static struct netisr_handler repi_nh = {

	.nh_name = "repi",
	.nh_handler = repi_nh_input,
	.nh_proto = NETISR_REPI,
	.nh_policy = NETISR_POLICY_SOURCE,
	.nh_dispatch = NETISR_DISPATCH_DIRECT,

};

MALLOC_DECLARE(M_REPI_HASH);
MALLOC_DEFINE(M_REPI_HASH, "repi_hash", "memory used by repi protocol in a hash table");

static void
repi_init(__unused void *args) {

	printf("REPI Initializing...\n");

	repi_stats.repi_input_dgrams_count = counter_u64_alloc(M_WAITOK);
	repi_stats.repi_output_dgrams_count = counter_u64_alloc(M_WAITOK);
	repi_stats.repi_forwarded_dgrams_count = counter_u64_alloc(M_WAITOK);
	repi_stats.repi_input_bytes_count = counter_u64_alloc(M_WAITOK);
	repi_stats.repi_output_bytes_count = counter_u64_alloc(M_WAITOK);
	repi_stats.repi_forwarded_bytes_count = counter_u64_alloc(M_WAITOK);

	counter_u64_zero(repi_stats.repi_input_dgrams_count);
	counter_u64_zero(repi_stats.repi_output_dgrams_count);
	counter_u64_zero(repi_stats.repi_forwarded_dgrams_count);
	counter_u64_zero(repi_stats.repi_input_bytes_count);
	counter_u64_zero(repi_stats.repi_output_bytes_count);
	counter_u64_zero(repi_stats.repi_forwarded_bytes_count);

	repi_hash = malloc(sizeof(int) * repi_hash_size, M_REPI_HASH, M_WAITOK);
	memset(repi_hash, -1, sizeof(int) * repi_hash_size);
	repi_hash_seed = arc4random();

	repi_bind_hash = malloc(sizeof(struct repi_pcb) * repi_bind_hash_size, M_REPI_HASH, M_WAITOK);
	memset(repi_bind_hash, 0, sizeof(struct repi_pcb) * repi_bind_hash_size);

	netisr_register(&repi_nh);

}

static void
repi_uninit(void) {

	counter_u64_free(repi_stats.repi_input_dgrams_count);
	counter_u64_free(repi_stats.repi_output_dgrams_count);
	counter_u64_free(repi_stats.repi_forwarded_dgrams_count);
	counter_u64_free(repi_stats.repi_input_bytes_count);
	counter_u64_free(repi_stats.repi_output_bytes_count);
	counter_u64_free(repi_stats.repi_forwarded_bytes_count);

	free(repi_hash, M_REPI_HASH);

	netisr_unregister(&repi_nh);

}

SYSINIT(repi_init, SI_SUB_PROTO_DOMAIN, SI_ORDER_ANY, repi_init, NULL);
SYSUNINIT(repi_uninit, SI_SUB_PROTO_DOMAIN, SI_ORDER_ANY, repi_uninit, NULL);

