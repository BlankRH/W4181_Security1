cat inputs/00006 >inputs/00007
echo >>inputs/00007
for i in {1..6700}
do
	cat alice.txt >>inputs/00007
done

echo >> inputs/00007
echo . >> inputs/00007
