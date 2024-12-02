//test.c

#channel.define A "chanA.ii"
#channel A
A

#channel.define B "chanB.ii"
#channel.close
#channel B
B
#channel.close B
C
#channel.include B
