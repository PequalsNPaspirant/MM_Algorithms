/*

Reference:
https://www.quora.com/How-does-a-relational-DBMS-internally-store-its-data

How DBMS store the data?

They use B+ trees. B+ tree is a special data structure allowing to efficiently store (i.e. access and update) a large sorted dictionary on a block storage device (i.e. HDD or SSD).

Sorted dictionary is, essentially, a phone book: it allows to locate a random entry by doing a tiny number of steps - i.e. without reading the whole book.

Block storage device can be described very well by a book too:
It's accessed page-by-page (block-by-block)
There is nearly fixed amount of information on each page (block)
You can quickly find a page (block) by its number without need to read anything else. This isn't quite correct for the book - usually you need to see the numbers of few nearby pages to do this. But let's assume we can do this in 1 step.
Reading next or previous page is very fast. This isn't a required property of block storage, but it's good to remember this, since actual block storages we use (HDDs and SSDs) really have it too, and DBMS rely on this fact.

B+ tree is an algorithm (i.e. a set of rules) allowing not just quickly find the entries in phone book, but also:
Add / edit and delete these entries (assuming you can use an eraser)
Maintain desirable page fill ratio - i.e. ensure that % of used space on all pages (except may be 1) in our phone book is higher than desirable limit (usually 75-90%).

As you know, database is a set of tables, and table is a set of indexes. And each index is, essentially, a B+ tree storing its data.

Few examples of how this structure can be mapped to something we know very well:

1. Files and folders:
Database = some root folder
Tables = its subfolders
Indexes = files in each subfolder

2. Bookcase:
Database = bookcase
Table = a book shelf in the bookcase
Index = a book on a shelf


So how table is actually stored as indexes? Let's assume each table has primary key (PK) - a set of columns uniquely identifying any row there. If some table doesn't, we can always add so-called surrogate primary key - a column storing e.g. a very precise time stamp for any particular row showing the moment it was added at.

Since any table has its own primary key (PK), we can store its data as a dictionary sorted by its primary key in B+ tree structure. E.g. if the original table is this one:

Name | Age | State
John | 25 | NY
Jane | 28 | CA
Alex | 33 | CA
Mike | 33 | NY

Its primary index has this structure:
'Alex' -> (33, 'CA')
'Jane' -> (28, 'CA')
'John' -> (25, 'NY')
'Mike' -> (33, 'NY')

As you see, I sorted it in alphabetical order: we store indexes as B+ trees, thus any index is a sorted dictionary as well.

So we know each table has at least primary index, which actually stores the table itself. But there can be other indexes called secondary indexes, that map selected set of columns (secondary index key) to a primary key. Let's see how index on State column (let's call it "IX_State") looks like:
'CA' -> 'Alex'
'CA' -> 'Jane'
'NY' -> 'John'
'NY' -> 'Mike'

It's sorted by State column first, and then by primary key (Name column): normally any index is fully sorted by its key and all columns from the primary key, which aren't part of the secondary key.

Also note that there is no Age column in this index - later I'll show some consequences of this.

How DBMS query the data efficiently?

Let's see how database engine executes this query:

Select Name, State
from People
where State = 'CA'
order by Age

Query translator transforms it to nearly this execution plan:

a = a sequence of all entries from People's table primary index
b = a sequence of entries from a, but where State = 'CA'
c = a sequence of entries from b, but sorted by Age
d = a sequence of entries from c, but only Name and State columns
result = d

a, b, c and d are sequences here - i.e. programming abstractions allowing to enumerate all the entries produced by each stage of query execution (an operation of the query execution plan).
Most of operations don't actually store these entries - in fact, they act like Unix pipes sequentially consuming some input and producing the output. Such operations are called streaming operations. Note that index range scan (AKA "index seek") and full index scan (AKA "full scan") are streaming operations, i.e. when a single entry or a sequential range of entries is extracted from index, DBMS doesn't need to even store the result.
But there are some non-streaming operations - they need to read and process the whole result before producing any output. E.g. sorting is non-streaming operation.

To fully clarify these concepts, let's see how above plan could look like, if our database engine would be written in Python:

a = db.get_index('People', 'PK').all()
b = (entry for entry in a if entry['State'] == 'CA')
c = sort(b, 'Age')
d = select(c, 'Name', 'State')
return d

a, b and d are streaming operations here: Index.all() extracts index range (full index), and two others are just Python sequences
c is non-streaming operation, since it requires the whole sequence to be loaded and sorted before it produces the first entry of result.

So it's easy to estimate how much RAM a particular query needs to execute: it's roughly the amount of RAM needed to store all intermediate results of non-streaming operations in query plan. You can assume virtual RAM is used when intermediates don't fit in physical RAM. Actually it's more complex - there are on-disk temporary tables, etc., but it's nearly the same in terms of expected performance.

You may find this is a suboptimal plan: it implies DBMS must read all entries from People's primary index, and also, store full c operation result, since sort is non-streaming operation.

But the good thing is: DBMS passes this initial query plan to a query optimizer, which should transform it to a nearly optimal plan. In our case this plan should be this:

a = entries from People's table IX_State secondary index with 'CA' key
b = a joined with People's table primary index by Name column
c = b sorted by Age
d = select only Name and State columns from c
result = d

Or as Python code:

pk = db.get_index('People', 'PK')
ix_state = db.get_index('People', 'IX_State')

a = ix_state.range(point('CA').inclusive(), point('CA').inclusive())
b = (pk.get(entry['Name']) for entry in a)
c = sort(b, 'Age')
d = select(c, 'Name', 'State')
return d

Why this plan is better? Because it processes only a limited number of rows - precisely, all rows with State == 'CA'. Unoptimized query plan processes all rows on steps a and b, so we can predict that optimized plan executes ~ 50x faster (assuming there are 50 states, and people are evenly distributed among them).

Can we improve it? Yes. You might notice that new plan has join operation (step b) - it's necessary, because IX_State doesn't have Age column, but we use it in sort operation. So the only thing DBMS can do here is to join primary index (which always contain all table columns) - as you see in Python's code, "join" here means to do one index lookup per each row in the original data set. Such type of join is called "loop join".

Despite the fact index lookups are fast, joins are relatively costly, since if you join a large data set, you can safely assume DBMS must read approx. one index page in the joined index per each row from the original set. I.e. basically, it will read way more data than it needs: index page usually contains hundreds of entries, but join will discard most of them and use just one from each page, because the chance joined rows come sequentially in the primary index is tiny.

So to improve the efficiency of this query, we can:

1. Add Age column to IX_State index - right after all existing key columns there.

So after adding this column the index should look like this:
('CA', 28) -> 'Jane'
('CA', 33) -> 'Alex'
('NY', 25) -> 'John'
('NY', 33) -> 'Mike'

When it's done, query optimizer won't need to join primary index, since IX_State already has all the data we need, and still can be used to evaluate "where State =='CA'" clause with index seek:

ix_state = db.get_index('People', 'IX_State')

a = ix_state.range(point('CA').inclusive(), point('CA').inclusive())
b = sort(a, 'Age')
c = select(b, 'Name', 'State')
return c

2. We can also remove order by Age clause from the original query - of course, if we don't really need it.


This was a very rough description of how DBMS work. Actually:
There are other types of indexes - e.g. hash and bitmap indexes. But they are used pretty rarely - e.g. InnoDB engine in MySQL supports just B+ trees, and as you know, many of top internet sites (Facebook, Wikipedia, and Quora as well) don't need others to run smoothly. Probably the only other type of index you need to know about is R-tree, which helps to query spatial data (so apps like Google Maps use similar indexes).
I said almost nothing about how query optimizer works, and I'd recommend to read about this too. But again, it's high-level description is simple: it tries to apply different transformations to the query plan to reduce query execution cost (i.e. ~ estimated amount of CPU and IO resources needed to execute the query). There are cases on which query optimizers are bad, so it's good to know about them. E.g. queries with few joins / subqueries are already hard for optimizers, so you can expect suboptimal plans here. Also, there are lots of cases when even optimal plans are hard.
Finally, it's good to know about transactions and transaction isolation. By some reason developers tend to totally ignore this part, but you can save a tremendous amount of time + write way safer code by properly employing these concepts.

*/