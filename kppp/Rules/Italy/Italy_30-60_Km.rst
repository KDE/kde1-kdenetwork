################################################################
#
# Ruleset for Italian Telecom
#
# written by Marco Giacomassi (giaco@computech.it)
#
################################################################

name=Italy_30-60_Km
currency_symbol=L.
currency_position=right 
currency_digits=0

default=(180.88,60)
on (monday..friday) between (8:00..18:30) use(403.41, 60)
on (monday..friday) between (18:30..22:00) use(227.29, 60)  
on (saturday) between (8:00..13:00) use (403.41, 60)
on (saturday) between (13:00..22:00) use (227.29, 60)
on (sunday) between (8:00..22:00) use (227.29, 60)