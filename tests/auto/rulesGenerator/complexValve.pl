:- use_module(library(clpfd)).

stackAutoPredicate(C_0,C_1,C_2,C_3,C_4,P_5,P_6,R_5,R_6,T_0_5,T_1_6,T_5_7,T_6_7,T_7_2,T_7_3,T_7_4,V_7):-
P_5 in -1 .. 1,
R_5 in 0 .. 999,
P_6 in -1 .. 1,
R_6 in 0 .. 999,
V_7 in 0 .. 2 \/ 4 .. 4,
C_0 in -30999 .. -30001 \/ -28999 .. -28001 \/ -26999 .. -26001 \/ -24999 .. -24001 \/ -22999 .. -22001 \/ -20999 .. -20001 \/ -18999 .. -18001 \/ -16999 .. -16001 \/ -14999 .. -14001 \/ -12999 .. -12001 \/ -10999 .. -10001 \/ -8999 .. -8001 \/ -6999 .. -6001 \/ -4999 .. -4001 \/ -2999 .. -2001 \/ -1999 .. -1001 \/ 0 .. 0 \/ 1001 .. 1999 \/ 2001 .. 2999 \/ 4001 .. 4999 \/ 6001 .. 6999 \/ 8001 .. 8999 \/ 10001 .. 10999 \/ 12001 .. 12999 \/ 14001 .. 14999 \/ 16001 .. 16999 \/ 18001 .. 18999 \/ 20001 .. 20999 \/ 22001 .. 22999 \/ 24001 .. 24999 \/ 26001 .. 26999 \/ 28001 .. 28999 \/ 30001 .. 30999,
C_1 in -29999 .. -29001 \/ -28999 .. -28001 \/ -25999 .. -25001 \/ -24999 .. -24001 \/ -21999 .. -21001 \/ -20999 .. -20001 \/ -17999 .. -17001 \/ -16999 .. -16001 \/ -13999 .. -13001 \/ -12999 .. -12001 \/ -9999 .. -9001 \/ -8999 .. -8001 \/ -5999 .. -5001 \/ -4999 .. -4001 \/ -2999 .. -2001 \/ -1999 .. -1001 \/ 0 .. 0 \/ 1001 .. 1999 \/ 2001 .. 2999 \/ 4001 .. 4999 \/ 5001 .. 5999 \/ 8001 .. 8999 \/ 9001 .. 9999 \/ 12001 .. 12999 \/ 13001 .. 13999 \/ 16001 .. 16999 \/ 17001 .. 17999 \/ 20001 .. 20999 \/ 21001 .. 21999 \/ 24001 .. 24999 \/ 25001 .. 25999 \/ 28001 .. 28999 \/ 29001 .. 29999,
C_2 in -27999 .. -27001 \/ -26999 .. -26001 \/ -25999 .. -25001 \/ -24999 .. -24001 \/ -19999 .. -19001 \/ -18999 .. -18001 \/ -17999 .. -17001 \/ -16999 .. -16001 \/ -11999 .. -11001 \/ -10999 .. -10001 \/ -9999 .. -9001 \/ -8999 .. -8001 \/ -4999 .. -4001 \/ -3999 .. -3001 \/ -2999 .. -2001 \/ -1999 .. -1001 \/ 0 .. 0 \/ 1001 .. 1999 \/ 2001 .. 2999 \/ 3001 .. 3999 \/ 4001 .. 4999 \/ 8001 .. 8999 \/ 9001 .. 9999 \/ 10001 .. 10999 \/ 11001 .. 11999 \/ 16001 .. 16999 \/ 17001 .. 17999 \/ 18001 .. 18999 \/ 19001 .. 19999 \/ 24001 .. 24999 \/ 25001 .. 25999 \/ 26001 .. 26999 \/ 27001 .. 27999,
C_3 in -23999 .. -23001 \/ -22999 .. -22001 \/ -21999 .. -21001 \/ -20999 .. -20001 \/ -19999 .. -19001 \/ -18999 .. -18001 \/ -17999 .. -17001 \/ -16999 .. -16001 \/ -8999 .. -8001 \/ -7999 .. -7001 \/ -6999 .. -6001 \/ -5999 .. -5001 \/ -4999 .. -4001 \/ -3999 .. -3001 \/ -2999 .. -2001 \/ -1999 .. -1001 \/ 0 .. 0 \/ 1001 .. 1999 \/ 2001 .. 2999 \/ 3001 .. 3999 \/ 4001 .. 4999 \/ 5001 .. 5999 \/ 6001 .. 6999 \/ 7001 .. 7999 \/ 8001 .. 8999 \/ 16001 .. 16999 \/ 17001 .. 17999 \/ 18001 .. 18999 \/ 19001 .. 19999 \/ 20001 .. 20999 \/ 21001 .. 21999 \/ 22001 .. 22999 \/ 23001 .. 23999,
C_4 in -16999 .. -16001 \/ -15999 .. -15001 \/ -14999 .. -14001 \/ -13999 .. -13001 \/ -12999 .. -12001 \/ -11999 .. -11001 \/ -10999 .. -10001 \/ -9999 .. -9001 \/ -8999 .. -8001 \/ -7999 .. -7001 \/ -6999 .. -6001 \/ -5999 .. -5001 \/ -4999 .. -4001 \/ -3999 .. -3001 \/ -2999 .. -2001 \/ -1999 .. -1001 \/ 0 .. 0 \/ 1001 .. 1999 \/ 2001 .. 2999 \/ 3001 .. 3999 \/ 4001 .. 4999 \/ 5001 .. 5999 \/ 6001 .. 6999 \/ 7001 .. 7999 \/ 8001 .. 8999 \/ 9001 .. 9999 \/ 10001 .. 10999 \/ 11001 .. 11999 \/ 12001 .. 12999 \/ 13001 .. 13999 \/ 14001 .. 14999 \/ 15001 .. 15999 \/ 16001 .. 16999,
T_0_5 in -30999 .. -30001 \/ -29999 .. -29001 \/ -28999 .. -28001 \/ -27999 .. -27001 \/ -26999 .. -26001 \/ -25999 .. -25001 \/ -24999 .. -24001 \/ -23999 .. -23001 \/ -22999 .. -22001 \/ -21999 .. -21001 \/ -20999 .. -20001 \/ -19999 .. -19001 \/ -18999 .. -18001 \/ -17999 .. -17001 \/ -16999 .. -16001 \/ -15999 .. -15001 \/ -14999 .. -14001 \/ -13999 .. -13001 \/ -12999 .. -12001 \/ -11999 .. -11001 \/ -10999 .. -10001 \/ -9999 .. -9001 \/ -8999 .. -8001 \/ -7999 .. -7001 \/ -6999 .. -6001 \/ -5999 .. -5001 \/ -4999 .. -4001 \/ -3999 .. -3001 \/ -2999 .. -2001 \/ -1999 .. -1001 \/ 0 .. 0 \/ 1001 .. 1999 \/ 2001 .. 2999 \/ 3001 .. 3999 \/ 4001 .. 4999 \/ 5001 .. 5999 \/ 6001 .. 6999 \/ 7001 .. 7999 \/ 8001 .. 8999 \/ 9001 .. 9999 \/ 10001 .. 10999 \/ 11001 .. 11999 \/ 12001 .. 12999 \/ 13001 .. 13999 \/ 14001 .. 14999 \/ 15001 .. 15999 \/ 16001 .. 16999 \/ 17001 .. 17999 \/ 18001 .. 18999 \/ 19001 .. 19999 \/ 20001 .. 20999 \/ 21001 .. 21999 \/ 22001 .. 22999 \/ 23001 .. 23999 \/ 24001 .. 24999 \/ 25001 .. 25999 \/ 26001 .. 26999 \/ 27001 .. 27999 \/ 28001 .. 28999 \/ 29001 .. 29999 \/ 30001 .. 30999,
T_5_7 in -30999 .. -30001 \/ -29999 .. -29001 \/ -28999 .. -28001 \/ -27999 .. -27001 \/ -26999 .. -26001 \/ -25999 .. -25001 \/ -24999 .. -24001 \/ -23999 .. -23001 \/ -22999 .. -22001 \/ -21999 .. -21001 \/ -20999 .. -20001 \/ -19999 .. -19001 \/ -18999 .. -18001 \/ -17999 .. -17001 \/ -16999 .. -16001 \/ -15999 .. -15001 \/ -14999 .. -14001 \/ -13999 .. -13001 \/ -12999 .. -12001 \/ -11999 .. -11001 \/ -10999 .. -10001 \/ -9999 .. -9001 \/ -8999 .. -8001 \/ -7999 .. -7001 \/ -6999 .. -6001 \/ -5999 .. -5001 \/ -4999 .. -4001 \/ -3999 .. -3001 \/ -2999 .. -2001 \/ -1999 .. -1001 \/ 0 .. 0 \/ 1001 .. 1999 \/ 2001 .. 2999 \/ 3001 .. 3999 \/ 4001 .. 4999 \/ 5001 .. 5999 \/ 6001 .. 6999 \/ 7001 .. 7999 \/ 8001 .. 8999 \/ 9001 .. 9999 \/ 10001 .. 10999 \/ 11001 .. 11999 \/ 12001 .. 12999 \/ 13001 .. 13999 \/ 14001 .. 14999 \/ 15001 .. 15999 \/ 16001 .. 16999 \/ 17001 .. 17999 \/ 18001 .. 18999 \/ 19001 .. 19999 \/ 20001 .. 20999 \/ 21001 .. 21999 \/ 22001 .. 22999 \/ 23001 .. 23999 \/ 24001 .. 24999 \/ 25001 .. 25999 \/ 26001 .. 26999 \/ 27001 .. 27999 \/ 28001 .. 28999 \/ 29001 .. 29999 \/ 30001 .. 30999,
T_7_3 in -30999 .. -30001 \/ -29999 .. -29001 \/ -28999 .. -28001 \/ -27999 .. -27001 \/ -26999 .. -26001 \/ -25999 .. -25001 \/ -24999 .. -24001 \/ -23999 .. -23001 \/ -22999 .. -22001 \/ -21999 .. -21001 \/ -20999 .. -20001 \/ -19999 .. -19001 \/ -18999 .. -18001 \/ -17999 .. -17001 \/ -16999 .. -16001 \/ -15999 .. -15001 \/ -14999 .. -14001 \/ -13999 .. -13001 \/ -12999 .. -12001 \/ -11999 .. -11001 \/ -10999 .. -10001 \/ -9999 .. -9001 \/ -8999 .. -8001 \/ -7999 .. -7001 \/ -6999 .. -6001 \/ -5999 .. -5001 \/ -4999 .. -4001 \/ -3999 .. -3001 \/ -2999 .. -2001 \/ -1999 .. -1001 \/ 0 .. 0 \/ 1001 .. 1999 \/ 2001 .. 2999 \/ 3001 .. 3999 \/ 4001 .. 4999 \/ 5001 .. 5999 \/ 6001 .. 6999 \/ 7001 .. 7999 \/ 8001 .. 8999 \/ 9001 .. 9999 \/ 10001 .. 10999 \/ 11001 .. 11999 \/ 12001 .. 12999 \/ 13001 .. 13999 \/ 14001 .. 14999 \/ 15001 .. 15999 \/ 16001 .. 16999 \/ 17001 .. 17999 \/ 18001 .. 18999 \/ 19001 .. 19999 \/ 20001 .. 20999 \/ 21001 .. 21999 \/ 22001 .. 22999 \/ 23001 .. 23999 \/ 24001 .. 24999 \/ 25001 .. 25999 \/ 26001 .. 26999 \/ 27001 .. 27999 \/ 28001 .. 28999 \/ 29001 .. 29999 \/ 30001 .. 30999,
T_6_7 in -30999 .. -30001 \/ -29999 .. -29001 \/ -28999 .. -28001 \/ -27999 .. -27001 \/ -26999 .. -26001 \/ -25999 .. -25001 \/ -24999 .. -24001 \/ -23999 .. -23001 \/ -22999 .. -22001 \/ -21999 .. -21001 \/ -20999 .. -20001 \/ -19999 .. -19001 \/ -18999 .. -18001 \/ -17999 .. -17001 \/ -16999 .. -16001 \/ -15999 .. -15001 \/ -14999 .. -14001 \/ -13999 .. -13001 \/ -12999 .. -12001 \/ -11999 .. -11001 \/ -10999 .. -10001 \/ -9999 .. -9001 \/ -8999 .. -8001 \/ -7999 .. -7001 \/ -6999 .. -6001 \/ -5999 .. -5001 \/ -4999 .. -4001 \/ -3999 .. -3001 \/ -2999 .. -2001 \/ -1999 .. -1001 \/ 0 .. 0 \/ 1001 .. 1999 \/ 2001 .. 2999 \/ 3001 .. 3999 \/ 4001 .. 4999 \/ 5001 .. 5999 \/ 6001 .. 6999 \/ 7001 .. 7999 \/ 8001 .. 8999 \/ 9001 .. 9999 \/ 10001 .. 10999 \/ 11001 .. 11999 \/ 12001 .. 12999 \/ 13001 .. 13999 \/ 14001 .. 14999 \/ 15001 .. 15999 \/ 16001 .. 16999 \/ 17001 .. 17999 \/ 18001 .. 18999 \/ 19001 .. 19999 \/ 20001 .. 20999 \/ 21001 .. 21999 \/ 22001 .. 22999 \/ 23001 .. 23999 \/ 24001 .. 24999 \/ 25001 .. 25999 \/ 26001 .. 26999 \/ 27001 .. 27999 \/ 28001 .. 28999 \/ 29001 .. 29999 \/ 30001 .. 30999,
T_1_6 in -30999 .. -30001 \/ -29999 .. -29001 \/ -28999 .. -28001 \/ -27999 .. -27001 \/ -26999 .. -26001 \/ -25999 .. -25001 \/ -24999 .. -24001 \/ -23999 .. -23001 \/ -22999 .. -22001 \/ -21999 .. -21001 \/ -20999 .. -20001 \/ -19999 .. -19001 \/ -18999 .. -18001 \/ -17999 .. -17001 \/ -16999 .. -16001 \/ -15999 .. -15001 \/ -14999 .. -14001 \/ -13999 .. -13001 \/ -12999 .. -12001 \/ -11999 .. -11001 \/ -10999 .. -10001 \/ -9999 .. -9001 \/ -8999 .. -8001 \/ -7999 .. -7001 \/ -6999 .. -6001 \/ -5999 .. -5001 \/ -4999 .. -4001 \/ -3999 .. -3001 \/ -2999 .. -2001 \/ -1999 .. -1001 \/ 0 .. 0 \/ 1001 .. 1999 \/ 2001 .. 2999 \/ 3001 .. 3999 \/ 4001 .. 4999 \/ 5001 .. 5999 \/ 6001 .. 6999 \/ 7001 .. 7999 \/ 8001 .. 8999 \/ 9001 .. 9999 \/ 10001 .. 10999 \/ 11001 .. 11999 \/ 12001 .. 12999 \/ 13001 .. 13999 \/ 14001 .. 14999 \/ 15001 .. 15999 \/ 16001 .. 16999 \/ 17001 .. 17999 \/ 18001 .. 18999 \/ 19001 .. 19999 \/ 20001 .. 20999 \/ 21001 .. 21999 \/ 22001 .. 22999 \/ 23001 .. 23999 \/ 24001 .. 24999 \/ 25001 .. 25999 \/ 26001 .. 26999 \/ 27001 .. 27999 \/ 28001 .. 28999 \/ 29001 .. 29999 \/ 30001 .. 30999,
T_7_2 in -30999 .. -30001 \/ -29999 .. -29001 \/ -28999 .. -28001 \/ -27999 .. -27001 \/ -26999 .. -26001 \/ -25999 .. -25001 \/ -24999 .. -24001 \/ -23999 .. -23001 \/ -22999 .. -22001 \/ -21999 .. -21001 \/ -20999 .. -20001 \/ -19999 .. -19001 \/ -18999 .. -18001 \/ -17999 .. -17001 \/ -16999 .. -16001 \/ -15999 .. -15001 \/ -14999 .. -14001 \/ -13999 .. -13001 \/ -12999 .. -12001 \/ -11999 .. -11001 \/ -10999 .. -10001 \/ -9999 .. -9001 \/ -8999 .. -8001 \/ -7999 .. -7001 \/ -6999 .. -6001 \/ -5999 .. -5001 \/ -4999 .. -4001 \/ -3999 .. -3001 \/ -2999 .. -2001 \/ -1999 .. -1001 \/ 0 .. 0 \/ 1001 .. 1999 \/ 2001 .. 2999 \/ 3001 .. 3999 \/ 4001 .. 4999 \/ 5001 .. 5999 \/ 6001 .. 6999 \/ 7001 .. 7999 \/ 8001 .. 8999 \/ 9001 .. 9999 \/ 10001 .. 10999 \/ 11001 .. 11999 \/ 12001 .. 12999 \/ 13001 .. 13999 \/ 14001 .. 14999 \/ 15001 .. 15999 \/ 16001 .. 16999 \/ 17001 .. 17999 \/ 18001 .. 18999 \/ 19001 .. 19999 \/ 20001 .. 20999 \/ 21001 .. 21999 \/ 22001 .. 22999 \/ 23001 .. 23999 \/ 24001 .. 24999 \/ 25001 .. 25999 \/ 26001 .. 26999 \/ 27001 .. 27999 \/ 28001 .. 28999 \/ 29001 .. 29999 \/ 30001 .. 30999,
T_7_4 in -30999 .. -30001 \/ -29999 .. -29001 \/ -28999 .. -28001 \/ -27999 .. -27001 \/ -26999 .. -26001 \/ -25999 .. -25001 \/ -24999 .. -24001 \/ -23999 .. -23001 \/ -22999 .. -22001 \/ -21999 .. -21001 \/ -20999 .. -20001 \/ -19999 .. -19001 \/ -18999 .. -18001 \/ -17999 .. -17001 \/ -16999 .. -16001 \/ -15999 .. -15001 \/ -14999 .. -14001 \/ -13999 .. -13001 \/ -12999 .. -12001 \/ -11999 .. -11001 \/ -10999 .. -10001 \/ -9999 .. -9001 \/ -8999 .. -8001 \/ -7999 .. -7001 \/ -6999 .. -6001 \/ -5999 .. -5001 \/ -4999 .. -4001 \/ -3999 .. -3001 \/ -2999 .. -2001 \/ -1999 .. -1001 \/ 0 .. 0 \/ 1001 .. 1999 \/ 2001 .. 2999 \/ 3001 .. 3999 \/ 4001 .. 4999 \/ 5001 .. 5999 \/ 6001 .. 6999 \/ 7001 .. 7999 \/ 8001 .. 8999 \/ 9001 .. 9999 \/ 10001 .. 10999 \/ 11001 .. 11999 \/ 12001 .. 12999 \/ 13001 .. 13999 \/ 14001 .. 14999 \/ 15001 .. 15999 \/ 16001 .. 16999 \/ 17001 .. 17999 \/ 18001 .. 18999 \/ 19001 .. 19999 \/ 20001 .. 20999 \/ 21001 .. 21999 \/ 22001 .. 22999 \/ 23001 .. 23999 \/ 24001 .. 24999 \/ 25001 .. 25999 \/ 26001 .. 26999 \/ 27001 .. 27999 \/ 28001 .. 28999 \/ 29001 .. 29999 \/ 30001 .. 30999,
(((((((((
	(
		(C_0 #= 0) 
	#\/
		((C_0 #> 0) #/\
		(((abs(C_0)) // 1000) #\= 1))
	) 
#\/
	((C_0 #< 0) #/\
	(((abs(C_0)) // 1000) #= 1))
) #/\
(C_0 #= (-1 * T_0_5))) #/\
((
	(
		(C_1 #= 0) 
	#\/
		((C_1 #> 0) #/\
		(((abs(C_1)) // 1000) #\= 2))
	) 
#\/
	((C_1 #< 0) #/\
	(((abs(C_1)) // 1000) #= 2))
) #/\
(C_1 #= (-1 * T_1_6)))) #/\
((
	(
		(C_2 #= 0) 
	#\/
		((C_2 #> 0) #/\
		(((abs(C_2)) // 1000) #\= 4))
	) 
#\/
	((C_2 #< 0) #/\
	(((abs(C_2)) // 1000) #= 4))
) #/\
(C_2 #= T_7_2))) #/\
((
	(
		(C_3 #= 0) 
	#\/
		((C_3 #> 0) #/\
		(((abs(C_3)) // 1000) #\= 8))
	) 
#\/
	((C_3 #< 0) #/\
	(((abs(C_3)) // 1000) #= 8))
) #/\
(C_3 #= T_7_3))) #/\
((
	(
		(C_4 #= 0) 
	#\/
		((C_4 #> 0) #/\
		(((abs(C_4)) // 1000) #\= 16))
	) 
#\/
	((C_4 #< 0) #/\
	(((abs(C_4)) // 1000) #= 16))
) #/\
(C_4 #= T_7_4))) #/\
(
	(
		((((P_5 #= 0) #/\
		(R_5 #= 0)) #/\
		(T_0_5 #= 0)) #/\
		(T_5_7 #= 0)) 
	#\/
		(((((P_5 #= 1) #/\
		(R_5 #= ((abs(T_0_5)) rem 1000))) #/\
		(T_0_5 #> 0)) #/\
		(T_5_7 #> 0)) #/\
		(T_5_7 #= T_0_5))
	) 
#\/
	(((((P_5 #= -1) #/\
	(R_5 #= ((abs(T_5_7)) rem 1000))) #/\
	(T_5_7 #< 0)) #/\
	(T_0_5 #< 0)) #/\
	(T_0_5 #= T_5_7))
)) #/\
(
	(
		((((P_6 #= 0) #/\
		(R_6 #= 0)) #/\
		(T_1_6 #= 0)) #/\
		(T_6_7 #= 0)) 
	#\/
		(((((P_6 #= 1) #/\
		(R_6 #= ((abs(T_1_6)) rem 1000))) #/\
		(T_1_6 #> 0)) #/\
		(T_6_7 #> 0)) #/\
		(T_6_7 #= T_1_6))
	) 
#\/
	(((((P_6 #= -1) #/\
	(R_6 #= ((abs(T_6_7)) rem 1000))) #/\
	(T_6_7 #< 0)) #/\
	(T_1_6 #< 0)) #/\
	(T_1_6 #= T_6_7))
)) #/\
(
	(
		(
			((V_7 #= 0) #/\
			(((T_5_7 #= 0) #/\
			(T_6_7 #= 0)) #/\
			(((T_7_3 #= 0) #/\
			(T_7_2 #= 0)) #/\
			(T_7_4 #= 0)))) 
		#\/
			(((V_7 #= 1) #/\
			(T_7_4 #= 0)) #/\
			((T_7_3 #= T_5_7) #/\
			(T_7_2 #= T_6_7)))
		) 
	#\/
		(((V_7 #= 2) #/\
		(T_7_4 #= 0)) #/\
		((T_7_3 #= T_6_7) #/\
		(T_7_2 #= T_5_7)))
	) 
#\/
	(((V_7 #= 4) #/\
	((T_7_3 #= 0) #/\
	(T_7_2 #= 0))) #/\
	(
		(
			(
				(((T_7_4 #> 0) #/\
				((T_5_7 #> 0) #/\
				(T_6_7 #> 0))) #/\
				(T_7_4 #= (((((abs(T_5_7)) // 1000) * 1000) + (((abs(T_6_7)) // 1000) * 1000)) + (((abs(T_5_7)) rem 1000) + ((abs(T_6_7)) rem 1000))))) 
			#\/
				(((T_7_4 #< 0) #/\
				((T_5_7 #< 0) #/\
				(T_6_7 #< 0))) #/\
				((T_5_7 #= (-1 * ((((abs(T_7_4)) // 1000) * 1000) + ((abs(T_7_4)) rem 1000)))) #/\
				(T_6_7 #= (-1 * ((((abs(T_7_4)) // 1000) * 1000) + ((abs(T_7_4)) rem 1000))))))
			) 
		#\/
			((T_5_7 #= 0) #/\
			(
				(((T_7_4 #> 0) #/\
				(T_6_7 #> 0)) #/\
				(T_7_4 #= ((((abs(T_6_7)) // 1000) * 1000) + ((abs(T_6_7)) rem 1000)))) 
			#\/
				(((T_7_4 #< 0) #/\
				(T_6_7 #< 0)) #/\
				(T_6_7 #= (-1 * ((((abs(T_7_4)) // 1000) * 1000) + ((abs(T_7_4)) rem 1000)))))
			))
		) 
	#\/
		((T_6_7 #= 0) #/\
		(
			(((T_7_4 #> 0) #/\
			(T_5_7 #> 0)) #/\
			(T_7_4 #= ((((abs(T_5_7)) // 1000) * 1000) + ((abs(T_5_7)) rem 1000)))) 
		#\/
			(((T_7_4 #< 0) #/\
			(T_5_7 #< 0)) #/\
			(T_5_7 #= (-1 * ((((abs(T_7_4)) // 1000) * 1000) + ((abs(T_7_4)) rem 1000)))))
		))
	))
)),
labeling([min(abs(P_5)),min(abs(P_6)),min(abs(V_7))],[P_5,P_6,V_7]),!.