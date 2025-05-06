在 MySQL 中，`GROUP BY` 子句用于将查询结果按照一个或多个列进行分组，然后可以对每个分组应用聚合函数（如 `COUNT()`、`SUM()`、`AVG()`、`MAX()`、`MIN()` 等）进行统计计算。下面结合你给出的 `pet` 表数据，详细介绍 `GROUP BY` 的用法。

### 基本语法
```sql
SELECT column1, aggregate_function(column2)
FROM table_name
WHERE condition
GROUP BY column1;
```
- `column1`：用于分组的列名。
- `aggregate_function(column2)`：对分组后的数据应用的聚合函数，例如 `COUNT(column2)`、`SUM(column2)` 等。
- `table_name`：要查询的表名。
- `condition`：可选的查询条件。

### 示例

#### 1. 按性别分组统计宠物数量
```sql
SELECT sex, COUNT(*)
FROM pet
GROUP BY sex;
```
**解释**：
 - 此查询将 `pet` 表中的数据按照 `sex` 列进行分组。
 - 对每个分组使用 `COUNT(*)` 函数统计该分组中的记录数量。
 - 执行该查询后，会得到每个不同 `sex` 值对应的宠物数量。

#### 2. 按性别分组统计平均年龄
```sql
SELECT sex, AVG(age)
FROM pet
GROUP BY sex;
```
**解释**：
 - 同样按照 `sex` 列对数据进行分组。
 - 对每个分组使用 `AVG(age)` 函数计算该分组中宠物的平均年龄。

### 结合 `HAVING` 子句进行筛选
当需要对分组后的结果进行筛选时，可以使用 `HAVING` 子句。`HAVING` 子句与 `WHERE` 子句类似，但 `WHERE` 子句用于筛选原始数据，而 `HAVING` 子句用于筛选分组后的结果。

#### 示例：筛选宠物数量大于 1 的性别分组
```sql
SELECT sex, COUNT(*)
FROM pet
GROUP BY sex
HAVING COUNT(*) > 1;
```
**解释**：
 - 先按照 `sex` 列对数据进行分组。
 - 对每个分组使用 `COUNT(*)` 函数统计宠物数量。
 - 使用 `HAVING` 子句筛选出宠物数量大于 1 的分组。

### 多列分组
你也可以使用多个列进行分组，这样会按照这些列的组合进行分组。

#### 示例：按性别和年龄分组统计宠物数量
```sql
SELECT sex, age, COUNT(*)
FROM pet
GROUP BY sex, age;
```
**解释**：
 - 按照 `sex` 和 `age` 列的组合对数据进行分组。
 - 对每个分组使用 `COUNT(*)` 函数统计该分组中的宠物数量。

### 注意事项
- **`GROUP BY` 列和 `SELECT` 列的关系**：在 `SELECT` 语句中，如果使用了 `GROUP BY` 子句，那么 `SELECT` 列表中的非聚合列必须包含在 `GROUP BY` 子句中。
- **性能影响**：`GROUP BY` 操作可能会对性能产生影响，尤其是在处理大量数据时。可以考虑使用索引来优化查询性能。

 