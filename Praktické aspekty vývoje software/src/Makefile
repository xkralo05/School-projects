#all target neobsahuje nic pretoze projekt je urobeny v pythone
#profiling sme spustali cez prikaz 'python3 -m cProfile profiling.py'
all:

pack:
	mkdir repo
	rsync -r ../* repo/
	rm -r ./repo/src/repo
	mkdir install
	cp -r ./SnakeJazzCalculator.deb install
	mkdir doc
	cp -r ../refman.pdf doc
	zip -r xponek00_xprecn00_xkralo05_xvalko11.zip install doc repo
	rm -r repo && rm -r install && rm -r doc
test:
	python3 -m pytest

install:
	sudo dpkg -i SnakeJazzCalculator.deb
	sudo apt install python3-pip	
	sudo pip3 install PyQt5

run:
	python3 main.py

app:
	SnakeJazzCalculator

uninstall:
	sudo dpkg -r SnakeJazzCalculator
	

doc:
	doxygen Doxyfile

clean:
	rm -f *.ps *.dvi *.aux *.toc *.idx *.ind *.ilg *.log *.out *.brf *.blg *.bbl refman.pdf