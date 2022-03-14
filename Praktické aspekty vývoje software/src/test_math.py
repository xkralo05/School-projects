## @file test_math.py
#  @brief Testovanie matematickej kniznice
#  @author Kristian Kralovic - xkralo05
#  @date Marec 2020

import mat_kniznica
import pytest

# test funkcie plus
def test_plus():
	#cele cisla s rovnakmym poctom cifier
	assert mat_kniznica.Library.plus(1,0) == 1
	assert mat_kniznica.Library.plus(0,1) == 1
	assert mat_kniznica.Library.plus(1,5) == 6
	assert mat_kniznica.Library.plus(10,45) == 55
	assert mat_kniznica.Library.plus(156,771) == 927
	assert mat_kniznica.Library.plus(9111,7711) == 16822
	assert mat_kniznica.Library.plus(99211,77821) == 177032
	assert mat_kniznica.Library.plus(777111,992111) == 1769222
	#cisla roznym poctom cifier
	assert mat_kniznica.Library.plus(22,330) == 352
	assert mat_kniznica.Library.plus(1,1532) == 1533
	assert mat_kniznica.Library.plus(9999,999010) == 1009009
	# zaporne + kladne
	assert mat_kniznica.Library.plus(-90,50) == -40
	assert mat_kniznica.Library.plus(-9077,71100)== 62023
	assert mat_kniznica.Library.plus(50,-90) == -40
	assert mat_kniznica.Library.plus(71100,-9077) == 62023
	# desatinne cisla 
	assert mat_kniznica.Library.plus(77.11,11.30) == 88.41
	assert mat_kniznica.Library.plus(0.32,0.75) == 1.07
	assert mat_kniznica.Library.plus(-1.5,8.9955) == 7.4955
	with pytest.raises(TypeError):
		mat_kniznica.Library.plus()

#test funkcie minus
def test_minus():
	#cele cisla s rovnakmym poctom cifier
	assert mat_kniznica.Library.minus(0,1) == -1
	assert mat_kniznica.Library.minus(1,0) == 1
	assert mat_kniznica.Library.minus(50,30) == 20
	assert mat_kniznica.Library.minus(1820,1200) == 620
	assert mat_kniznica.Library.minus(70305,56871) == 13434
	assert mat_kniznica.Library.minus(156201,872168) == -715967
 	#cele cisla s roznym poctom cifier
	assert mat_kniznica.Library.minus(11,851) == -840
	assert mat_kniznica.Library.minus(774,3) == 771
	assert mat_kniznica.Library.minus(9999999,81138) == 9918861
	assert mat_kniznica.Library.minus(111,8741) == -8630
	# kladne - zaporne 
	assert mat_kniznica.Library.minus(80,-80) == 160
	assert mat_kniznica.Library.minus(-10,90) == -100
	assert mat_kniznica.Library.minus(-880,-9501) == 8621
	# desatinne cisla
	assert mat_kniznica.Library.minus(-0.50,10.44126) == -10.94126
	assert mat_kniznica.Library.minus(0.31,-70.69) == 71
	assert mat_kniznica.Library.minus(-0.11,-9.78) == 9.67
	
	with pytest.raises(TypeError):
		mat_kniznica.Library.minus()

#test funkcie nasobenie
def test_nasobenie():
	#cele cisla s rovnakmym poctom cifier (a*b)
	assert mat_kniznica.Library.nasobenie(10,15) == 150
	assert mat_kniznica.Library.nasobenie(771,876) == 675396
	assert mat_kniznica.Library.nasobenie(88741,78452) == 6961908932
	#cele cisla s roznym poctom cifier
	assert mat_kniznica.Library.nasobenie(77,0) == 0
	assert mat_kniznica.Library.nasobenie(787,12) == 9444	
	assert mat_kniznica.Library.nasobenie(12,787) == 9444
	# klasne - zaporne 
	assert mat_kniznica.Library.nasobenie(-10,7) == -70
	assert mat_kniznica.Library.nasobenie(70,-9) == -630
	assert mat_kniznica.Library.nasobenie(-9,-9) == 81
	# desatinne cisla
	assert mat_kniznica.Library.nasobenie(0.23,0.23) == 0.0529
	assert mat_kniznica.Library.nasobenie(-1.51,7.001) == -10.57151
	assert mat_kniznica.Library.nasobenie(-7.701,-0.78) == 6.00678
	
	with pytest.raises(TypeError):
		mat_kniznica.Library.nasobenie()



#test funkcie delenie 
def test_delenie() :
	#cele cisla s rovnakmym poctom cifier (a/b)
	assert mat_kniznica.Library.delenie(0,1) == 0
	assert mat_kniznica.Library.delenie(9,3) == 3	
	
	assert mat_kniznica.Library.delenie(1810.0,1580.0) == 1.14556962
	assert mat_kniznica.Library.delenie(777770.0,847150) == 0.91810187
	#cele cisla s roznym poctom cifier
	assert mat_kniznica.Library.delenie(789,50) ==15.78
	assert mat_kniznica.Library.delenie(8877,100) == 88.77
	# kladne - zaporne 
	assert mat_kniznica.Library.delenie(-1000,10) == -100
	assert mat_kniznica.Library.delenie(1000,-10) == -100
	assert mat_kniznica.Library.delenie(-1000,-10) == 100
	# desatinne cisla
	assert mat_kniznica.Library.delenie(0.25,0.25) == 1
	assert mat_kniznica.Library.delenie(-0.25,0.25000000) == -1
	assert mat_kniznica.Library.delenie(1.5,0.25) == 6
	assert mat_kniznica.Library.delenie(-1.5,0.25) == -6
	assert mat_kniznica.Library.delenie(16.60,8.30) == 2
	# delenie 0
	with pytest.raises(ValueError):
		mat_kniznica.Library.delenie(0,0)
	with pytest.raises(ValueError):
		mat_kniznica.Library.delenie(1,0)	
	with pytest.raises(ValueError):
		mat_kniznica.Library.delenie(7481,0)
	with pytest.raises(TypeError):
		mat_kniznica.Library.delenie()

#test funkcie faktorial
def test_faktorial() :
	assert mat_kniznica.Library.faktorial(0) == 1
	assert mat_kniznica.Library.faktorial(5) == 120
	assert mat_kniznica.Library.faktorial(10) == 3628800
	with pytest.raises(ValueError):
		mat_kniznica.Library.faktorial(-5)
	with pytest.raises(ValueError):	
		mat_kniznica.Library.faktorial(1.5)
	with pytest.raises(ValueError):	
		mat_kniznica.Library.faktorial(70)
	with pytest.raises(TypeError):	
		mat_kniznica.Library.faktorial()



#test funkcie umocnovanie
#prve cislo v zatvorke je cislo zaklad mocniny , druhe je exponent.
def test_umocnovanie() :
	assert mat_kniznica.Library.umocnovanie(2,2) == 4
	assert mat_kniznica.Library.umocnovanie(2,1) == 2
	assert mat_kniznica.Library.umocnovanie(10,0) == 1
	assert mat_kniznica.Library.umocnovanie(-2,2) == 4
	assert mat_kniznica.Library.umocnovanie(-2,3) == -8
	with pytest.raises(ValueError):	
		mat_kniznica.Library.umocnovanie(2,1.5)
	with pytest.raises(TypeError):	
		mat_kniznica.Library.umocnovanie()	
	with pytest.raises(ValueError):
		mat_kniznica.Library.umocnovanie(10,-3)		

#test funkcie odmocniny 
#prve cislo v zatvorke je odmocnenec a druhe cislo je odmocnitel 
def test_odmocnina() :
	assert mat_kniznica.Library.odmocnina(9,2) == 3
	assert mat_kniznica.Library.odmocnina(20,3) == 2.71441762
	assert mat_kniznica.Library.odmocnina(1000000,10) == 3.98107171
	assert mat_kniznica.Library.odmocnina(9,-3) == 0.48074986 
	assert mat_kniznica.Library.odmocnina(0,6) == 0
	assert mat_kniznica.Library.odmocnina(1,8) == 1
	assert mat_kniznica.Library.odmocnina(-6,3) == -1.81712059
	assert mat_kniznica.Library.odmocnina(-6,-3) == -0.55032121
	assert mat_kniznica.Library.odmocnina(-6,1.5) == -3.30192725
	with pytest.raises(ValueError):	
		mat_kniznica.Library.odmocnina(6,0)
	with pytest.raises(ValueError):	
		mat_kniznica.Library.odmocnina(-6,6)	
	with pytest.raises(TypeError):	
		mat_kniznica.Library.odmocnina()	

#test funkcie logaritmus
def test_logaritmus() : 
	assert mat_kniznica.Library.logaritmus(1) == 0 
	assert mat_kniznica.Library.logaritmus(10.2) == 1.008600171762
	assert mat_kniznica.Library.logaritmus(0.5) == -0.301029995664
	assert mat_kniznica.Library.logaritmus(8888) == 3.948804045933
	with pytest.raises(ValueError) :	
		mat_kniznica.Library.logaritmus(0)
	with pytest.raises(ValueError):	
		mat_kniznica.Library.logaritmus(-10)
	with pytest.raises(TypeError):	
		mat_kniznica.Library.logaritmus()





