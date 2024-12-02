//test.c

#channel.define A "chanA.ii"
#channel A
#channel.write A "string 1"

#channel.write "string 2"
#channel.write A

#channel.close
#channel.write A "string 3"

#channel.destroy A
#channel.write A "string 4"
