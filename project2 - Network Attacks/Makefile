UNAME := $(shell uname)
ifeq ($(UNAME), Darwin)
	SED = gsed
else
	SED = sed
endif
all:
	# Remove header from packet
	$(SED) -b -e "s/00000256//g" packet > payload
	# Decryt payload to ticket
	openssl rsautl -decrypt -inkey private.pem -in payload -out ticket
	# Change ticket for admin
	$(SED) -e "s/false/true/g" ticket > ticket_admin
	# Encrypt ticket to payload
	openssl rsautl -encrypt -inkey public.pem -pubin -in ticket_admin -out payload_admin
	# Add header to payload
	$(SED) -b -e "1s/^/00000256/" payload_admin > packet_admin
run:
	# Send packet to server
	cat packet_admin - |  nc 140.113.194.85 49175
clean:
	rm payload ticket
	rm ticket_admin payload_admin packet_admin
