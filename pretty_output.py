some MAGENTA text
from colorama import Fore, Back, Style

print(Style.BRIGHT + "and in bright")
print(Fore.RED + "some red text")
print(Fore.CYAN + "some cyan text")
print(Fore.BLUE + "some BLUE text")
print(Fore.YELLOW + "some YELLOW text")
print(Fore.GREEN + "some GREEN text")
print(Fore.MAGENTA + "some MAGENTA text")
#print(Back.GREEN + "and with a green background")
print(Style.DIM + "and in dim text")
print(Style.RESET_ALL)
print("back to normal now!!")


from prettytable import PrettyTable

#print(Style.BRIGHT + "and in bright")
table = PrettyTable(["name", "score"])
table.add_row([Fore.MAGENTA + "Bob", 67])
table.add_row(["grizzly", 45])
table.add_row(["Tom of Caerbannog", 78])
table.add_row(["cat", 34])
table.add_row(["Tony", 39])
table.add_row(["dolphin", 45])
table.add_row(["albatross", 24])
table.sort_key("name")
table.reversesort = True

print(table)
print(Style.RESET_ALL)
