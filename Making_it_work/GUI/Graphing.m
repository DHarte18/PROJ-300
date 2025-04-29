freshData = readtable("C:\Users\demil\Documents\GitHub\PROJ-300\Testing_stuff\Test2.txt");
H = height(freshData);
selLapTable = table;    %(Time, FLdis, FMdis, FRdis, BLdis, BMdis, BRdis, FLang, FRang, BLang, BRang);
lapSelect = 2;
a=1;
while H >= a
    rowSelTable = freshData(a,1);
    rowSelArray = table2array(rowSelTable);
    if rowSelArray == lapSelect
        selLapAppend = freshData(a,2:12);
        selLapTable = [selLapTable;selLapAppend];
        b = array2table(a);
        a = a + 1;
    else
        a = a + 1;
    end
end

Timeline(selLapTable);