################################################################
#
# Ruleset for Italian Telecom ( iva compresa )
#
# written by Marco Giacomassi <giaco@computech.it>
#
################################################################

name=Italy_Local_Area
currency_symbol=L.
currency_position=right 
currency_digits=0

default=(152.4,400)
on (monday..friday) between (8:00..18:30) use(152.4,220)
on (saturday) between(8:00..13:00) use (152.4, 220)
