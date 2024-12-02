//test.c

#channel.define A "chanA.ii"
#channel.header A HEADER
#channel A
A

#channel.define B "chanB.ii"
#channel.header B HEADER
#channel B
#channel A
#channel.destroy B
