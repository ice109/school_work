set k /0 * 7/;
set j(k) /1 * 6/;

parameter g(j)  /1 1300,
                 2 1400,
                 3 1000,
                 4 800,
                 5 1700,
                 6 1900/;
parameter c(j)  /1 100,
                 2 105,
                 3 110,
                 4 115,
                 5 110,
                 6 110/;


parameter  sc /4/,
           ws /1/,
           wi /1/,
           wp /1/,
           invPenalty /100/;

positive variables p(k), i(k), s(j), t(k);
i.l('0')= 100;
i.l('7') = 100;
p.l('0') = 1000;
t.l('0') = 1100;
free variable obj;

equations sales,inventory,objdef,totalstock;
totalstock(k)$(ord(k) gt 2).. t(k) =e= p(k-1)+i(k-1);
sales(j).. s(j) =l= t(j);
inventory(j).. i(j) =e= t(j) - s(j);
objdef.. obj =e= sum(j,p(j)*c(j)+sc*i(j)+ws*sqr(s(j)-g(j))+wi*sqr(i(j)-invPenalty)+wp*sqr(p(j)-p(j-1)));

model production /all/


solve production using nlp minimizing obj;
