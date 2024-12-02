//raw.c

// Valid raw strings
R"x(aaa)bbb)x"

R"x(aaa
)bbb)x"

R"prefix(aaa
)bbb prefix)prefix"


// Ill-formed raw strings
R"aaa"
R"(aaa)bbb)"

