在 MySQL 中，聚集函数（也称为聚合函数）用于对一组值进行计算并返回单个值。这些函数通常与 `GROUP BY` 子句一起使用，用于对数据进行分组统计。以下是一些常用的 MySQL 聚集函数：

### 1. `COUNT()`
- **功能**：用于统计记录的数量。
- **示例**：
```sql
-- 统计 `users` 表中的记录总数
SELECT COUNT(*) FROM users;

-- 统计 `users` 表中 `age` 列不为空的记录数量
SELECT COUNT(age) FROM users;
```
- **解释**：`COUNT(*)` 会统计表中的所有记录，而 `COUNT(column_name)` 只统计指定列不为 `NULL` 的记录数量。

### 2. `SUM()`
- **功能**：用于计算指定列中数值的总和。
- **示例**：
```sql
-- 计算 `orders` 表中 `amount` 列的总和
SELECT SUM(amount) FROM orders;
```
- **解释**：`SUM()` 函数只能用于数值类型的列。

### 3. `AVG()`
- **功能**：用于计算指定列中数值的平均值。
- **示例**：
```sql
-- 计算 `employees` 表中 `salary` 列的平均值
SELECT AVG(salary) FROM employees;
```
- **解释**：`AVG()` 函数同样只能用于数值类型的列，并且会忽略 `NULL` 值。

### 4. `MAX()`
- **功能**：用于返回指定列中的最大值。
- **示例**：
```sql
-- 查找 `products` 表中 `price` 列的最大值
SELECT MAX(price) FROM products;
```
- **解释**：`MAX()` 函数可以用于数值、字符串和日期类型的列。对于字符串类型，它会返回按字典序排序后的最后一个值；对于日期类型，它会返回最新的日期。

### 5. `MIN()`
- **功能**：用于返回指定列中的最小值。
- **示例**：
```sql
-- 查找 `students` 表中 `score` 列的最小值
SELECT MIN(score) FROM students;
```
- **解释**：`MIN()` 函数与 `MAX()` 函数类似，可以用于数值、字符串和日期类型的列。

### 与 `GROUP BY` 子句结合使用
聚集函数经常与 `GROUP BY` 子句一起使用，以便对数据进行分组统计。例如：
```sql
-- 统计每个部门的员工数量
SELECT department, COUNT(*) 
FROM employees 
GROUP BY department;

-- 计算每个类别的产品平均价格
SELECT category, AVG(price) 
FROM products 
GROUP BY category;
```
在上述示例中，`GROUP BY` 子句将数据按照指定的列进行分组，然后对每个组应用聚集函数进行统计。

### 注意事项
- **忽略 `NULL` 值**：大多数聚集函数会忽略 `NULL` 值，例如 `COUNT()`、`SUM()`、`AVG()`、`MAX()` 和 `MIN()`。
- **性能影响**：使用聚集函数可能会对查询性能产生影响，特别是在处理大量数据时。可以考虑使用索引来提高查询性能。