################################################################
#
# Ruleset for Italian Telecom
# based on pppcosts-0.5
#
# written by Mario Weilguni <mweilguni@sime.com>
# NOT YET TESTED
#
################################################################

name=Italy_Local_Area
currency_symbol=L.
currency_position=right 
currency_digits=0

default=(151.13,400)
on (monday..friday) between (8:30..18:30) use(151.13,220)
on (saturday) between(8:00..13:00) use (151.13, 220)
