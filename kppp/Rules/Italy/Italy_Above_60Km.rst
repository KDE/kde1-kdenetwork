################################################################
#
# Ruleset for Italian Telecom (iva compresa)
#
# written by Marco Giacomassi (giaco@computech.it)
#
################################################################

name=Italy_Above_60Km
currency_symbol=L.
currency_position=right 
currency_digits=0

default=(201.11,60)
on (monday..friday) between (8:00..18:30) use(453.39, 60)
on (monday..friday) between (18:30..22:00) use(274.89, 60)  
on (saturday) between (8:00..13:00) use (453.39, 60)
on (saturday) between (13:00..22:00) use (274.89, 60)
on (sunday) between (8:00..22:00) use (274.89, 60)