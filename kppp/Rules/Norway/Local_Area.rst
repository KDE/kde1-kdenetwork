################################################################
#
# Ruleset for Telenor Norway
# Local Area
# Created 97/09/20 by Arne Coucheron <arneco@online.no>
#
################################################################
name=Norway_Local_Area
currency_symbol=Kr
currency_position=left
currency_digits=2
per_connection=0.40
minimum_costs=0.0
default=(0.14,60)
on (monday..friday) between (8:00..17:00) use (0.25,60)
