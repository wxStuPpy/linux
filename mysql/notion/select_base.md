
### 插入数据（INSERT）

#### 1. 插入单条记录
- **语法**：
```sql
INSERT INTO table_name (column1, column2, ...)
VALUES (value1, value2, ...);
```
- **示例**：假设存在一个 `students` 表，包含 `id`、`name`、`age` 列，要插入一条新记录：
```sql
INSERT INTO students (id, name, age)
VALUES (1, 'Alice', 20);
```
如果要插入的列包含表中的所有列，并且列的顺序和表定义一致，还可以省略列名列表：
```sql
INSERT INTO students
VALUES (2, 'Bob', 22);
```

#### 2. 插入多条记录
- **语法**：
```sql
INSERT INTO table_name (column1, column2, ...)
VALUES (value1_1, value1_2, ...),
       (value2_1, value2_2, ...),
       ...;
```
- **示例**：
```sql
INSERT INTO students (id, name, age)
VALUES (3, 'Charlie', 21),
       (4, 'David', 23);
```

### 删除数据（DELETE）

#### 1. 删除满足条件的记录
- **语法**：
```sql
DELETE FROM table_name
WHERE condition;
```
- **示例**：要删除 `students` 表中 `age` 大于 22 的记录：
```sql
DELETE FROM students
WHERE age > 22;
```

#### 2. 删除表中的所有记录
如果要删除表中的所有记录，可以省略 `WHERE` 子句：
```sql
DELETE FROM students;
```
不过需要注意，这种操作会删除表中的所有数据，但表结构依然存在。如果想同时删除表结构和数据，可以使用 `DROP TABLE` 语句。

### 更新数据（UPDATE）

#### 1. 更新单条记录或满足条件的记录
- **语法**：
```sql
UPDATE table_name
SET column1 = value1, column2 = value2, ...
WHERE condition;
```
- **示例**：要将 `students` 表中 `id` 为 1 的学生的 `age` 更新为 21：
```sql
UPDATE students
SET age = 21
WHERE id = 1;
```

#### 2. 更新所有记录
如果省略 `WHERE` 子句，`UPDATE` 语句会更新表中的所有记录：
```sql
UPDATE students
SET age = age + 1;
```
这个语句会将 `students` 表中所有学生的年龄加 1。

### 注意事项
- **数据完整性**：在进行插入、删除和更新操作时，要确保操作不会破坏数据库的数据完整性，例如不要删除或更新有关联关系的数据。
- **备份数据**：在执行大量数据的删除或更新操作之前，建议先备份数据，以防误操作导致数据丢失。
- **性能影响**：对于大规模的数据操作，可能会对数据库性能产生影响，可以考虑分批处理或者使用事务来提高性能和数据的一致性。 

 1.select name from pet where age >= 1 order  by age desc,birth desc limit 3;
 2.show create table pet;
 3.desc pet;
 4.select * from pet where sex is not null;

 在 MySQL 中，对表进行增加列、删除列和修改列的操作是常见的表结构调整需求，下面分别为你介绍相关操作的语法和示例。

### 增加列（ADD COLUMN）

#### 语法
```sql
ALTER TABLE table_name
ADD COLUMN column_name data_type [constraint] [FIRST | AFTER existing_column];
```
- `table_name`：要操作的表名。
- `column_name`：要添加的列名。
- `data_type`：列的数据类型，例如 `INT`、`VARCHAR` 等。
- `constraint`：可选的列约束，如 `NOT NULL`、`DEFAULT` 等。
- `FIRST`：将新列添加到表的第一列。
- `AFTER existing_column`：将新列添加到指定列之后。

#### 示例
假设存在一个 `employees` 表，现在要为其添加一个 `email` 列，数据类型为 `VARCHAR(100)`，允许为空：
```sql
ALTER TABLE employees
ADD COLUMN email VARCHAR(100);
```
若要将新列添加到 `name` 列之后：
```sql
ALTER TABLE employees
ADD COLUMN email VARCHAR(100) AFTER name;
```
若要为新列设置默认值：
```sql
ALTER TABLE employees
ADD COLUMN email VARCHAR(100) DEFAULT 'example@example.com';
```

### 删除列（DROP COLUMN）

#### 语法
```sql
ALTER TABLE table_name
DROP COLUMN column_name;
```
- `table_name`：要操作的表名。
- `column_name`：要删除的列名。

#### 示例
若要从 `employees` 表中删除 `email` 列：
```sql
ALTER TABLE employees
DROP COLUMN email;
```

### 修改列（MODIFY COLUMN 或 CHANGE COLUMN）

#### 1. 使用 `MODIFY COLUMN` 修改列的数据类型或约束
- **语法**
```sql
ALTER TABLE table_name
MODIFY COLUMN column_name new_data_type [constraint];
```
- **示例**
假设要将 `employees` 表中 `age` 列的数据类型从 `INT` 修改为 `SMALLINT`：
```sql
ALTER TABLE employees
MODIFY COLUMN age SMALLINT;
```
若要将 `email` 列设置为 `NOT NULL`：
```sql
ALTER TABLE employees
MODIFY COLUMN email VARCHAR(100) NOT NULL;
```

#### 2. 使用 `CHANGE COLUMN` 修改列名、数据类型或约束
- **语法**
```sql
ALTER TABLE table_name
CHANGE COLUMN old_column_name new_column_name new_data_type [constraint];
```
- **示例**
若要将 `employees` 表中 `age` 列改名为 `employee_age`，并将数据类型修改为 `TINYINT`：
```sql
ALTER TABLE employees
CHANGE COLUMN age employee_age TINYINT;
```

### 注意事项
- **数据丢失风险**：在修改列的数据类型时，要确保新的数据类型能够容纳原有的数据，否则可能会导致数据丢失。例如，将 `INT` 类型的列修改为 `TINYINT` 时，如果原列中存在较大的整数，这些数据可能会被截断。
- **性能影响**：对表结构进行修改的操作（如增加列、删除列等）可能会对数据库性能产生影响，尤其是在处理大型表时。建议在业务低峰期进行此类操作。
- **备份数据**：在进行重要的表结构修改之前，建议先备份数据，以防操作失误导致数据丢失或损坏。 