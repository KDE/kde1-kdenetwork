################################################################
#
# Local calls in Hungary
# Valid all way in 1998 (I hope)
# by Tom a Hawk <warhawk@galaxycorp.com>
#
################################################################
name=Hungary_local

################################################################
# currency settings
################################################################

currency_symbol=Ft
currency_position=left
currency_digits=2

################################################################
# connection settings
################################################################

per_connection=0
minimum_costs=0

default=(13, 86)

on (monday..friday) between (0:00..4:59) use (13, 328)
on (monday..friday) between (5:00..6:59) use (13, 208)
on (monday..friday) between (7:00..8:59) use (13, 91)
on (monday..friday) between (9:00..14:59) use (13, 86)
on (monday..friday) between (15:00..17:59) use (13, 91)
on (monday..friday) between (18:00..21:59) use (13, 208)
on (monday..friday) between (22:00..23:59) use (13, 328)

on (saturday..sunday) between (0:00..4:59) use (13, 328)
on (saturday..sunday) between (5:00..21:59) use (13, 208)
on (saturday..sunday) between (22:00..23:59) use (13, 328)

