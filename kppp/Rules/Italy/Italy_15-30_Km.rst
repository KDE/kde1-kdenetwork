################################################################
#
# Ruleset for Italian Telecom (iva compresa)
#
# written by Marco Giacomassi (giaco@computech.it)
#
################################################################

name=Italy_15-30_Km
currency_symbol=L.
currency_position=right 
currency_digits=0

default=(113.05, 60)
on (monday..friday) between (8:00..13:00) use(245.14, 60)
on (monday..friday) between (13:00..18:30) use(227.29, 60)
on (monday..friday) between (18:30..22:00) use(146.37, 60)  
on (saturday) between (8:00..13:00) use (227.29, 60)
on (saturday) between (13:00..22:00) use (146.37, 60)
on (sunday) between (8:00..22:00) use (146.37, 60)