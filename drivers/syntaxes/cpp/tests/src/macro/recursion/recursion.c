// Recursive macros

// direct recursion
#define REC		rec=REC
REC

// indirect recursion
#define REC1	rec2=REC2
#define REC2	rec1=REC1
REC1
REC2

