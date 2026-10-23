
-- Лабораторная работа № 3
-- Проверка работы базы данных SpaceTrainerMonitor

-- 1. Список компонентов оборудования

SELECT
    id,
    name,
    current_status
FROM components
ORDER BY id;


-- 2. Измерения с названиями компонентов

SELECT
    c.name AS component,
    p.name AS parameter,
    m.value,
    p.unit,
    m.timestamp,
    m.status_code
FROM measurements m
JOIN parameters p ON m.parameter_id = p.id
JOIN components c ON p.component_id = c.id
ORDER BY c.name, m.timestamp DESC;


-- 3. Поиск измерений за допустимыми пределами

SELECT
    c.name AS component,
    p.name AS parameter,
    m.value AS measured_value,
    p.min_value,
    p.max_value,
    p.unit
FROM measurements m
JOIN parameters p ON m.parameter_id = p.id
JOIN components c ON p.component_id = c.id
WHERE
    (p.min_value IS NOT NULL AND m.value < p.min_value)
    OR
    (p.max_value IS NOT NULL AND m.value > p.max_value)
ORDER BY c.name;


-- 4. История диагностических событий

SELECT
    c.name AS component,
    e.event_type,
    e.description,
    e.severity,
    e.timestamp
FROM diagnostic_events e
JOIN components c ON e.component_id = c.id
ORDER BY e.timestamp DESC;


-- 5. Количество измерений по компонентам

SELECT
    c.name AS component,
    COUNT(m.id) AS measurement_count
FROM components c
LEFT JOIN parameters p ON p.component_id = c.id
LEFT JOIN measurements m ON m.parameter_id = p.id
GROUP BY c.id, c.name
ORDER BY c.name;


-- 6. Компоненты с проблемным состоянием

SELECT
    id,
    name,
    current_status
FROM components
WHERE current_status IN ('WARNING', 'ERROR')
ORDER BY name;
