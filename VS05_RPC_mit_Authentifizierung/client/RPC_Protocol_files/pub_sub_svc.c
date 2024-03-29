/*
 * Please do not edit this file.
 * It was generated using rpcgen.
 */

#include "pub_sub.h"
#include <stdio.h>
#include <stdlib.h>
#include <rpc/pmap_clnt.h>
#include <string.h>
#include <memory.h>
#include <sys/socket.h>
#include <netinet/in.h>

#ifndef SIG_PF
#define SIG_PF void(*)(int)
#endif

static void
pubsubprog_1(struct svc_req *rqstp, register SVCXPRT *transp)
{
	union {
		param set_channel_1_arg;
		param subscribe_1_arg;
		param unsubscribe_1_arg;
		param publish_1_arg;
		user get_session_1_arg;
		param validate_1_arg;
		sessionid invalidate_1_arg;
	} argument;
	char *result;
	xdrproc_t _xdr_argument, _xdr_result;
	char *(*local)(char *, struct svc_req *);

	switch (rqstp->rq_proc) {
	case NULLPROC:
		(void) svc_sendreply (transp, (xdrproc_t) xdr_void, (char *)NULL);
		return;

	case set_channel:
		_xdr_argument = (xdrproc_t) xdr_param;
		_xdr_result = (xdrproc_t) xdr_short;
		local = (char *(*)(char *, struct svc_req *)) set_channel_1_svc;
		break;

	case subscribe:
		_xdr_argument = (xdrproc_t) xdr_param;
		_xdr_result = (xdrproc_t) xdr_short;
		local = (char *(*)(char *, struct svc_req *)) subscribe_1_svc;
		break;

	case unsubscribe:
		_xdr_argument = (xdrproc_t) xdr_param;
		_xdr_result = (xdrproc_t) xdr_short;
		local = (char *(*)(char *, struct svc_req *)) unsubscribe_1_svc;
		break;

	case publish:
		_xdr_argument = (xdrproc_t) xdr_param;
		_xdr_result = (xdrproc_t) xdr_short;
		local = (char *(*)(char *, struct svc_req *)) publish_1_svc;
		break;

	case get_session:
		_xdr_argument = (xdrproc_t) xdr_user;
		_xdr_result = (xdrproc_t) xdr_sessionid;
		local = (char *(*)(char *, struct svc_req *)) get_session_1_svc;
		break;

	case validate:
		_xdr_argument = (xdrproc_t) xdr_param;
		_xdr_result = (xdrproc_t) xdr_short;
		local = (char *(*)(char *, struct svc_req *)) validate_1_svc;
		break;

	case invalidate:
		_xdr_argument = (xdrproc_t) xdr_sessionid;
		_xdr_result = (xdrproc_t) xdr_short;
		local = (char *(*)(char *, struct svc_req *)) invalidate_1_svc;
		break;

	default:
		svcerr_noproc (transp);
		return;
	}
	memset ((char *)&argument, 0, sizeof (argument));
	if (!svc_getargs (transp, (xdrproc_t) _xdr_argument, (caddr_t) &argument)) {
		svcerr_decode (transp);
		return;
	}
	result = (*local)((char *)&argument, rqstp);
	if (result != NULL && !svc_sendreply(transp, (xdrproc_t) _xdr_result, result)) {
		svcerr_systemerr (transp);
	}
	if (!svc_freeargs (transp, (xdrproc_t) _xdr_argument, (caddr_t) &argument)) {
		fprintf (stderr, "%s", "unable to free arguments");
		exit (1);
	}
	return;
}

int
main (int argc, char **argv)
{
	register SVCXPRT *transp;

	pmap_unset (PUBSUBPROG, PUBSUBVERS);

	transp = svcudp_create(RPC_ANYSOCK);
	if (transp == NULL) {
		fprintf (stderr, "%s", "cannot create udp service.");
		exit(1);
	}
	if (!svc_register(transp, PUBSUBPROG, PUBSUBVERS, pubsubprog_1, IPPROTO_UDP)) {
		fprintf (stderr, "%s", "unable to register (PUBSUBPROG, PUBSUBVERS, udp).");
		exit(1);
	}

	transp = svctcp_create(RPC_ANYSOCK, 0, 0);
	if (transp == NULL) {
		fprintf (stderr, "%s", "cannot create tcp service.");
		exit(1);
	}
	if (!svc_register(transp, PUBSUBPROG, PUBSUBVERS, pubsubprog_1, IPPROTO_TCP)) {
		fprintf (stderr, "%s", "unable to register (PUBSUBPROG, PUBSUBVERS, tcp).");
		exit(1);
	}

	svc_run ();
	fprintf (stderr, "%s", "svc_run returned");
	exit (1);
	/* NOTREACHED */
}
