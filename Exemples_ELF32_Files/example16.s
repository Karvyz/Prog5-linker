@  Algorithme de multiplication par addition et decalage
@  
@  Principe : si a = Sigma de i=0 a n-1 des a_i * 2^i
@  alors  resultat = Sigme de i=0 a n-1 des (a_i * 2^i) * b_i
@  Pour a entier relatif : a_(n-1) est a mutiplier par -2^(n-1)
@ 
@  Principe : pour chaque bit i de a a 1, ajouter   b << i
@            (ajouter b si a_i et faire b = b*2 a chaque passage)
@
@ int mult (int a, int b) {
@   int resultat;
@   unsigned int au;     // a pris comme un naturel (pour >>)
@ 
@   au = a; resultat = 0;
@   if (a <0) {          // se ramener au cas a >= 0
@     au = -a; b = -b;
@   } 
@   while (au != 0) {
@     if ((au & 1) != 0) // ajouter b si a_0 == 1
@        resultat = resultat + b;
@     b = b * 2;         // ou b = b << 1
@     au = au / 2;       // ou au = au >> 1
@   }
@   return resultat;
@ }
@ 

@ Convention d'appel : a : r0, b : r1 valeur retour : r0
@                      resultat : r2    au: confondu avec a: r0
   .text
   .global   mult
mult:
   stmfd   sp!, {r1,r2}   @ sauver aussi b par precaution
   mov   r2,#0        @ resultat =0
   cmp   r0,#0        @ if (a<0)
   rsblt   r0,r0,#0   @   au = -a
   rsblt   r1,r1,#0   @   b = -b
    b   condtq         @ while (au != 0)
tq: tst   r0,#1        @ if ((au&1) != 0)
      addne   r2,r2,r1      @ resultat = resultat + b
      mov   r1,r1, LSL #1   @ b = b *2
      mov   r0,r0, LSR #1   @ au = au /2
condtq: cmp   r0,#0
    bne   tq
   mov   r0,r2      @ return resultat
   ldmfd   sp!,{r1,r2}
   mov   pc,lr

