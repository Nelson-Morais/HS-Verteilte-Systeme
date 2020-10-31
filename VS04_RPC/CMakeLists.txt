cmake_minimum_required(VERSION 3.16)
project(VS04_RPC C)

set(CMAKE_C_STANDARD 99)

add_executable(RPC_Client client/client.c client/RPC_Protocol_files/pub_sub_clnt.c client/RPC_Protocol_files/pub_sub_xdr.c)
add_executable(RPC_Receiver client/receiver.c client/RPC_Protocol_files/pub_sub_deliv_svc.c client/RPC_Protocol_files/pub_sub_deliv_xdr.c)
add_executable(RPC_Server server/dispatcher.c server/RPC_Protocol_files/pub_sub_xdr.c server/RPC_Protocol_files/pub_sub_deliv_clnt.c server/RPC_Protocol_files/pub_sub_deliv_xdr.c server/RPC_Protocol_files/pub_sub_svc.c)