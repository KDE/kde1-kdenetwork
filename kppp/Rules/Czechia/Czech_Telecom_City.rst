################################################################
# Czech Telecom rate ruleset
#
# created 98/01/29 by Miroslav Flídr <flidr@kky.zcu.cz>
#
# no extensive testing...
################################################################

name=Czech_Telecom_City
currency_symbol=Kc
currency_position=right 
currency_digits=2
per_connection=0.0
minimum_costs=0.0
default=(2.25, 360)

# (for CITY Tarif)
on (monday..friday)   between (7:00..18:00)  use (2.25, 180)
