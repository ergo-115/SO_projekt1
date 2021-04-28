#!/bin/bash

#skrypt utworzy pliki z miejsca wywołania, i tylko z tego miejsca może je usunąć
#./createDir c by stworzyc strukture katalogow
#./createDir r by usunac utworzone foldery
if [ -z "$1" ] || ([ "$1" != "c" ] && [ "$1" != "r" ])
then 
	echo "Uzycie ./createDir c (by utworzyc katalogi) lub ./createDir r (by usunąć katalogi)"
	exit 1
fi


if [ "$1" == "r" ]
then
	if [[ -d aaa ]]
	then
		rm -rf aaa
		echo "katalog aaa usunięto"
	fi
	if [[ -d zzz ]]
	then
		rm -rf zzz
		echo "katalog zzz usunięto"
	fi
exit 0
fi
#przykladowe uzycie jesli chodzi o sam program: source aaa destination zzz
if [ "$1" == "c" ]
then

mkdir -p aaa/bbb/ccc/aaa
mkdir -p aaa/bbb/ccc/aab
mkdir -p aaa/bbb/aa{c,e}
#chmod 555 aaa/bbb/aae
#mkdir -p aaa/aa{f,g}
touch aaa/a.txt | echo "I can’t believe I got fired from the calendar factory. All I did was take a day off." > aaa/a.txt
touch aaa/b.txt | echo "I wasn’t originally going to get a brain transplant, but then I changed my mind." > aaa/b.txt
touch aaa/d.txt | echo "I wondered why the Frisbee was getting bigger. Then it hit me." > aaa/d.txt
touch aaa/bbb/aac/gg.txt
touch aaa/bbb/ccc/aaa/gz.sh | echo "Most people are shocked when they find out how bad an electrician I am." > aaa/bbb/ccc/aaa/gz.sh

#ln -s aaa/bbb/xy.txt gglink.txt
#SYMBOLICZNY LINK KATALOG I PLIK?
cp -pr aaa zzz
touch aaa/bbb/ccc/x.txt | echo "I put a new freezer next to the refrigerator, now they’re just chilling." > aaa/bbb/ccc/x.txt #powinien byc skopiowany do miejsca docelowego
touch aaa/bbb/aae/xx.txt | echo "I would make jokes about the sea, but they’re too deep." > aaa/bbb/aae/xx.txt #powinien byc skopiowany do miejsca docelowego
touch aaa/bbb/xy.txt | echo "I don’t trust staircases. They’re always up to something." > aaa/bbb/xy.txt #powinien byc skopiowany do miejsca docelowego
echo "I lost my mood ring the other day. I’m not sure how to feel about it." >> zzz/b.txt #do nadpisania, rozmiar inny
touch zzz/c.txt | echo "Some clown opened the door for me this morning. That was a nice jester." > zzz/c.txt #do skasowania
touch -d "10 hours ago" zzz/d.txt #do nadpisania, data modyfikacji inna
touch zzz/e.txt
mkdir -p zzz/{a,b} #do skasowania katalogi
chmod 111 zzz/e.txt #do skasowania
touch zzz/bbb/ccc/f.txt #do skasowania
touch zzz/bbb/aac/g.txt #do skasowania
touch zzz/bbb/aae/h.txt | echo "I couldn’t quite remember how to throw a boomerang, but eventually, it came back to me." > zzz/bbb/aae/h.txt #do skasowania
touch zzz/bbb/z.txt
chmod 111 zzz/bbb/z.txt #do skasowania
touch -d "30 hours ago" zzz/bbb/aac/gg.txt #do nadpisania, data modyfikacji inna
echo "I’m reading a book about anti-gravity. It’s impossible to put down." >> zzz/bbb/ccc/aaa/gz.sh #do nadpisania, rozmiar inny
mkdir -p aaa/gk #do skopiowania
mkdir -p aaa/gkk #do skopiowania
touch aaa/gkk/qw.txt #do skopiowania
#ponizsza linijka sluzy do stworzenia pliku 1,1GB
#dd if=/dev/zero of=file.txt count=1024 bs=1048576 
echo "Utworzono strukture katalogow"
exit 0
fi
