################################################################
# Malaysia Internet Rate
# 
# 25/8/98 -by Binary ( 1010110@netexecutive.com )
################################################################
################################################################
#
# Both Jaring and TMnet user will have the same rate
#
################################################################
name=MY_Rate

# defines RM (Ringgit Malaysia) to be used as currency
# symbol is RM
currency_symbol=RM

# Define the position of the currency symbol.
# RM 1.00 is normally used.
currency_position=left 

# Define the number of significat digits.
# (not absolutely needed, default is "2"
currency_digits=2



################################################################
# connection settings
################################################################

# minimum costs per per connection. If the costs of a phone
# call are less than this value, this value is used instead
minimum_costs=0.01

# Every 60 seconds, RM 0.01 or 1 cent are added to the bill"
default=(0.01, 60)
