################################################################
#
# Ruleset for Italian Telecom (iva compresa)
#
# written by Marco Giacomassi (giaco@computech.it)
#
################################################################

name=Italy_0-15_Km
currency_symbol=L.
currency_position=right 
currency_digits=0

default=(182.4, 60)
on (monday..friday) between (8:00..18:30) use(406.8, 60)
on (saturday) between (8:00..13:00) use (406.8, 60)
