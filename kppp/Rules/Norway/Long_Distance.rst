################################################################
#
# Ruleset for Telenor Norway
# Long Distance
# Created 97/09/20 by Arne Coucheron <arneco@online.no>
#
################################################################
name=Norway_Long_Distance
currency_symbol=Kr
currency_position=left
currency_digits=2
per_connection=0.40
minimum_costs=0.0
default=(0.50,60)
on (monday..friday) between (8:00..17:00) use (0.60,60)
