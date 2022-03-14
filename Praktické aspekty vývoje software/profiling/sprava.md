Stručné zhrnutie profilingu:
Z výstupov pri rôznych vstupoch čísel (použité vstupy možno nájsť v zložke vstupy)
ktoré sú zhrnuté v profiling_out.txt 
najviac času zaberá: operacia plus a operacia minus z knižnice mat_kniznica,
ktoré sú ale volané najviac-krát (pri vstupe 1000 čísel operacia plus 2000-krát
a operacia umocnovanie 1001-krát), ale aj pri týchto volaniach je čas strávený 
v nich pod 0.005ms. Povedal by som teda že by pri optimalizácií sa nemusíme 
zamerať na operácie z mat knižnice, pretože bežia pomerne rýchlo,

Dodatočne by som neskôr spustil profiler nad príkladmi ktoré by použili funkcie 
na parsovanie mat. výrazov z mainu, ktorý spája GUI a mat_kniznicu, aby sme zistili
či parsovanie beží dostatočne rýchlo.
Na rozbehnutie profilingu nad príkladom výpočtu výběrové směrodatné odchylky 
tieto funkcie neboli potrebné a preto nie sú zahrnuté vo výstupoch profilingu.
