################################################################
# 
# Ruleset for French Telecom
# Appel local 
# $Id$
# (C) 1997 Czo <sirol@asim.lip6.fr>
#
################################################################


# Tarifs France telecom au 13/07/96
#
# Une unite depuis un poste d'abonne = 0.742 F TTC quelleque soit la periode
# Zone locale
#
# Periode ROUGE        : 180 sec
# Periode BLANCHE      : 270
# Periode BLUE         : 360
# Periode BLEU_NUIT    : 540

name=France_Local_Area
currency_symbol=F
currency_position=right 
currency_digits=2




################################################################
# connection settings
################################################################

# NOTE: rules are applied from top to bottom - so only the
#       LAST matching rule is applied. Make sure that the
#       more "general" (more often used) rules are before
#       more specific rules.

# This is charged whenever you connect. If you don't have to
# pay per-connection, use "0" here or comment it out.
per_connection=0.0

# minimum costs per per connection. If the costs of a phone
# call are < this value, this value is used instead
minimum_costs=0.0

# This is the default rule which is used when no other rule
# applies. The first component "0.8" is the price of one
# "unit", while "180" is the duration of such a unit in seconds.
default=(0.742, 180)

flat_init_costs=(100,10)

on (monday..sunday)   between (06:00..08:00) use (0.742, 360)
on (monday..sunday)   between (21:30..22:30) use (0.742, 360)
on (monday..sunday)   between (22:30..00:00) use (0.742, 540)
on (monday..sunday)   between (00:00..06:00) use (0.742, 540)
on (sunday)           between (06:00..22:30) use (0.742, 360)


on (monday..saturday) between (08:00..12:30) use (0.742, 180)
on (monday..saturday) between (12:30..13:30) use (0.742, 270)
on (monday..friday)   between (13:30..18:00) use (0.742, 180)
on (monday..friday)   between (18:00..21:30) use (0.742, 270)
on (saturday)         between (13:30..22:30) use (0.742, 360)

# fete legales
on (01/01, easter+1, 05/01, 05/08, easter+38, easter+50, 07/14, 08/15, 11/01, 11/11, 12/25) between (06:00..22:30) use (0.742, 360)
on (01/01, easter+1, 05/01, 05/08, easter+38, easter+50, 07/14, 08/15, 11/01, 11/11, 12/25) between (22:30..00:00) use (0.742, 540)
on (01/01, easter+1, 05/01, 05/08, easter+38, easter+50, 07/14, 08/15, 11/01, 11/11, 12/25) between (00:00..06:00) use (0.742, 540)
