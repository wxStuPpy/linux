在 MySQL 里，`DISTINCT` 是一个用于查询的关键字，它的主要作用是去除查询结果里的重复行，保证返回的每一行数据都是唯一的。下面为你详细介绍 `DISTINCT` 的用法和相关注意事项。

### 基本语法
`DISTINCT` 关键字一般紧跟在 `SELECT` 之后，其基本语法如下：
```sql
SELECT DISTINCT column1, column2, ...
FROM table_name
WHERE condition;
```
- `column1, column2, ...`：你要查询的列名。
- `table_name`：你要查询的表名。
- `condition`：可选的查询条件。

### 示例

#### 1. 单列使用 `DISTINCT`
假设存在一个 `employees` 表，其中有 `department` 列，若要查询所有不同的部门，可以使用如下 SQL 语句：
```sql
SELECT DISTINCT department
FROM employees;
```
这条语句会返回 `employees` 表中 `department` 列的所有不同值，去除了重复的部门名称。

#### 2. 多列使用 `DISTINCT`
若要同时查询多列的不同组合，可以在 `SELECT` 后面列出多个列名，示例如下：
```sql
SELECT DISTINCT department, job_title
FROM employees;
```
此语句会返回 `department` 和 `job_title` 列的所有不同组合，只有当两列的值都相同时才会被视为重复行。

### 与聚集函数结合使用
`DISTINCT` 还能和聚集函数（如 `COUNT()`、`SUM()` 等）搭配使用，用于统计不同值的数量或进行其他计算。

#### 1. 统计不同值的数量
若要统计 `employees` 表中不同部门的数量，可以使用如下 SQL 语句：
```sql
SELECT COUNT(DISTINCT department)
FROM employees;
```
这条语句会返回 `department` 列中不同值的数量。

#### 2. 计算不同值的总和
假设 `employees` 表中有 `salary` 列，若要计算不同薪资的总和，可以使用如下 SQL 语句：
```sql
SELECT SUM(DISTINCT salary)
FROM employees;
```
此语句会先去除 `salary` 列中的重复值，然后计算这些不同薪资的总和。

### 注意事项
- **性能影响**：使用 `DISTINCT` 会增加数据库的处理负担，尤其是在处理大量数据时，可能会导致查询性能下降。若可能的话，可以考虑使用索引来优化查询。
- **所有列参与判断**：当使用 `DISTINCT` 对多列进行操作时，只有所有指定列的值都相同，才会被视为重复行。
- **`NULL` 值处理**：`DISTINCT` 会将 `NULL` 值视为相同的值，即所有 `NULL` 只会保留一个。 