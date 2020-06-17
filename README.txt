Read Me:

Character - Inventory 

UI - Inventory - Character

inventory stores uniform arrays of integers that represent individual items. They are referred to by index value, so that if one knows the
index of an item in the ItemsId array, they can use that index to pull metadata from the isWeapon or contentID arrays

Foliage God is to Plant as enemyHandler is to enemybaseclass and as itempickup is to resource spawner
all create data points and metadata for their respective class instances and store them in CSV files, one for each of the three and for each block
Each block has a seed data file for each: (items, foliage, enemies).



Good to know: a "block" or block number is an abstract representation of a landscape tile's bounds and is used to refer to its contents