################################################################
# This is rule set for Poland local phone cost (up to 25 km).
#
# Piotr Haberko <gumis@wsi.edu.pl>
# or Piotr Haberko <gumis@gumis.silesia.linux.org.pl>
################################################################


################################################################
#
# NAME OF THE RULESET. This is NEEDED for accounting purposes.
#
################################################################
name=Poland

################################################################
# currency settings
################################################################
#
#
currency_symbol=PLN

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
minimum_costs=0.0

# Therefore the following rule means: "Every 180 seconds 0.17
# PLN are added to the bill"
# taryfa nocna
default=(0.29, 360)
# taryfa dzienna
on (monday..sunday) between (8:00..21:59) use (0.29, 180)

