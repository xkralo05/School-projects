## @file mat_kniznica
#  @brief Matematicka kniznica pre projekt 2 do IVS
#  @author Kristian Kralovic <xkralo05>
#  @date April 2020
import math
#@var desatinne
#Udava pocet cislic za desatinnou ciarkou 
desatinne=8
#@var desatinne_log
#Udava pocet cislic za desatinnou ciarkou pri vypocte logaritmu
desatinne_log=12
class Library:
	## Funkcia plus sluzi na vypocet aritmetickej operacie plus
	#@brief  funkcia vypocita x+y
	#@param hx pomocna premenna pre zistenie ci "x" je cislo(int,float)
	#@param hy pomocna premenna pre zistenie ci "y" je cislo(int,float)
	#@param x prvy scitanec
	#@patam y druhy scitanec
	#@return funkcia vrati sucet "x" a "y" zaokruhleny na 8 desatinnych miest
	#@exception  TypeError Ak "x" alebo "y" niesu cisla alebo neboli zadane
	#
	def plus(x,y):
		hx=isinstance(x,(int, float))
		if not hx :
			raise TypeError("Zly pocet argumentov!")
		hy=isinstance(y,(int, float))
		if not hy :
			raise TypeError("Zly pocet argumentov!")
		return round (x+y,desatinne)

	## Funkcia minus sluzi na vypovet aritmetickej operacie minus
	#@brief  funkcia vypocita x-y
	#@param hx pomocna premenna pre zistenie ci "x" je cislo(int,float)
	#@param hy pomocna premenna pre zistenie ci "y" je cislo(int,float)
	#@param x mensenec
	#@patam y mensitel 
	#@return funkcia vrati rozdiel "x" a "y" zaokruhleny na 8 desatinnych miest
	#@exception  TypeError Ak "x" alebo "y" niesu cisla alebo neboli zadane
	#
	def minus(x,y):
		hx=isinstance(x,(int, float))
		if not hx :
			raise TypeError("Zly pocet argumentov!")
		hy=isinstance(y,(int, float))
		if not hy :
			raise TypeError("Zly pocet argumentov!")
		return round (x-y,desatinne)

	## Funkcia sluzi na vypocet aritmetickej operacie nasobenie 
	#@brief  funkcia vypocita x*y
	#@param hx pomocna premenna pre zistenie ci "x" je cislo(int,float)
	#@param hy pomocna premenna pre zistenie ci "y" je cislo(int,float)
	#@param x prvy cinitel
	#@patam y druhy cinitel
	#@return funkcia vrati sucin "x" a "y" zaokruhleny na 8 desatinnych miest
	#@exception  TypeError Ak "x" alebo "y" niesu cisla alebo neboli zadane
	#
	def nasobenie(x,y):
		hx=isinstance(x,(int, float))
		if not hx :
			raise TypeError("Zly pocet argumentov!")
		hy=isinstance(y,(int, float))
		if not hy :
			raise TypeError("Zly pocet argumentov!")	
		return round (x*y ,desatinne)

	## Funkcia sluzi na vypocet aritmetickej operacie delenie 
	#@brief  funkcia vypocita x/y
	#@param hx pomocna premenna pre zistenie ci "x" je cislo(int,float)
	#@param hy pomocna premenna pre zistenie ci "y" je cislo(int,float)
	#@param x delenec
	#@patam y delitel
	#@return funkcia vrati podiel "x" a "y" zaokruhleny na 8 desatinnych miest
	#@exception  TypeError Ak "x" alebo "y" niesu cisla alebo neboli zadane
	#@exception  ZeroDivisionError ak y == 0
	#
	def delenie(x,y):
		if y == 0 :
			raise ValueError("syntax error")
		hx=isinstance(x,(int, float))
		if not hx :
			raise ValueError("syntax error")
		hy=isinstance(y,(int, float))
		if not hy :
			raise ValueError("syntax error")	

		return round( x/y ,desatinne)	

	## Funkcia sluzi na rekurzivny vypocet faktorialu zadaneho cisla
	#@brief  funkcia vypocita x!
	#@param hx pomocna premenna pre zistenie ci x je cislo(int,float)
	#@param x Cislo z ktoreho pocitame faktorial
	#@return funkcia vrati faktorial cisla "X" alebo 1 ak x==0
	#@exception  TypeError Ak "x"  nieje cislo alebo nebolo zadane
	#@exception  Ak x > 69 alebo x < 0
	#

	def faktorial(x):
		if x < 0 :
			raise ValueError("math error")
		if x > 69 :
			raise ValueError("syntax error")
		if not x == int(x):
			raise ValueError("math error")
		hx=isinstance(x,(int, float))
		if not hx :
			raise TypeError("Zly pocet argumentov!")
		if x == 0 :
			return 1
		else:
			return x*Library.faktorial(x-1)

	## Funkcia sluzi na vypocet mocniny lubovolneho cisla umocneneho lubovolnym prirodzenym cislom
	#@brief  funkcia vypocita x^y
	#@param hx pomocna premenna pre zistenie ci "x" je cislo(int,float)
	#@param hy pomocna premenna pre zistenie ci "y" je cislo(int,float)
	#@param x zaklad mocniny
	#@param y mocnitel
	#@return funkcia vrati x^y
	#@exception  TypeError Ak "x" alebo "y" niesu cisla alebo neboli zadane
	#@exception  Ak y<0 - Cislo "y" nieje prirodzene cislo
	#
	def umocnovanie(x,y):
		if y < 0 :
			raise ValueError("math error")
		if not y ==	int(y):
			raise ValueError("syntax error")
		hx=isinstance(x,(int, float))
		if not hx :
			raise TypeError("Zly pocet argumentov!")
		hy=isinstance(y,(int, float))
		if not hy :
			raise TypeError("Zly pocet argumentov!")	
		return round(x**y, desatinne)		

	## Funkcia sluzi na vypocet obecnej odmocniny
	#@brief  funkcia vypocita vseobecnu odmocninu
	#@param hx pomocna premenna pre zistenie ci "x" je cislo(int,float)
	#@param hy pomocna premenna pre zistenie ci "y" je cislo(int,float)
	#@param x odmocnenec
	#@param y odmocnitel
	#@return funkcia vrati vysledok odmocnovania zaokruhleny n 8 desatinnych miest
	#@exception  Valuerror Ak "x" alebo "y" nies u cisla alebo neboli zadane
	#@exception  Ak zaklad mocniny(x) je zaporne cislo a odmocnitel(y) je parne cislo
	#@exception  Ak odmocnitel (y) == 0 
	#
	def odmocnina(x,y):
		hx=isinstance(x,(int, float))
		if not hx :
			raise TypeError("Zly pocet argumentov!")
		
		hy=isinstance(y,(int, float))
		if not hy :
			raise ValueError("syntax error")	
		if x < 0 and (y%2 == 0):
			raise ValueError("math error")
		if y == 0 :
			raise ValueError("math error")
		if x < 0 and (y%2 != 0):
			x=x*(-1)
			return round(-((x)**(1/y)),desatinne)
		else:
			return round((x)**(1/y) ,desatinne)

	## Funkcia sluzi na vypocet prirodzeneho logaritmu 
	#@brief  funkcia vypocita ln(x)
	#@param hx pomocna premenna pre zistenie ci "x" je cislo(int,float)
	#@param x cislo z kotreho robime ln
	#@return funkcia vrati vysledok ln(x) zaokruhleny na 12 desatinnych miest
	#@exception  TypeError Ak "x" alebo nieje cislo alebo nebolo zadane
	#@exception  Ak y<=0 - Cislo "y" nieje prirodzene cislo
	#
	def logaritmus(x):
		hx=isinstance(x,(int, float))
		if not hx :
			raise ValueError("syntax error")
		if x <= 0 :
			raise ValueError("math error")
		return round(math.log10(x),desatinne_log)	