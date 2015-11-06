#!/bin/bash
UNAME_S=$(uname -s)
if [[ ${UNAME_S} =~ Darwin* ]]
then
	alias SED='gsed'
else
	alias SED='sed'
fi
# Remove header from packet
SED -b -e "s/00000256//g" packet > payload
# Decryt payload to ticket
openssl rsautl -decrypt -inkey private.pem -in payload -out ticket
# Change ticket for admin
SED -e "s/false/true/g" ticket > ticket_admin
# Encrypt ticket to payload
openssl rsautl -encrypt -inkey public.pem -pubin -in ticket_admin -out payload_admin
# Add header to payload
SED -b -e "1s/^/00000256/" payload_admin > packet_admin
# Send packet to server
cat packet_admin - |  nc 140.113.194.85 49175