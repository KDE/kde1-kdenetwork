# Accounting Rule Set for Portugal Telecom's Local calls
#
# By:
#	Ricardo Ferreira aka Storm
#	storm@mail.telepac.pt

#Name of the Rule Set
#
name=default

################################################################
# currency settings
################################################################

# defines Esc. (Escuudo) to be used as currency
# symbol (not absolutely needed, default = "$")
currency_symbol=Esc.

# Define the position of the currency symbol.
# (not absolutely needed, default is "right")
currency_position=right 

# Define the number of significat digits.
# (not absolutely needed, default is "2"
currency_digits=2

################################################################
# connection settings
################################################################

# NOTE: rules are applied from top to bottom - the
#       LAST matching rule is the one used for the
#       cost computations.

# This is charged whenever you connect. If you don't have to
# pay per-connection, use "0" here or comment it out.
per_connection=0.0


# minimum costs per per connection. If the costs of a phone
# call are less than this value, this value is used instead
minimum_costs=13.50

# This is the default rule which is used when no other rule
# applies. The first component is the price of one
# "unit", while the second is the duration in seconds.
###########################
# Defaults to the most economical mode. In effect all sundays,holidays and
# weekdays from 22:00 to 8:00
###########################
default=(13.50, 540)

#
# more complicated rules:
#

#########################
# Apply more expensive rule if not on Sunday and between 8:00 - 22:00
#########################
on (monday..saturday) between (8:00..22:00) use (13.50, 180)
