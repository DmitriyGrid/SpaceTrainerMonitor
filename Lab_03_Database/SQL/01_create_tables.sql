
-- Лабораторная работа № 3
-- Создание базы данных SpaceTrainerMonitor

-- 1. Компоненты тренажёра

CREATE TABLE components (
    id INTEGER GENERATED ALWAYS AS IDENTITY PRIMARY KEY,
    name VARCHAR(100) NOT NULL,
    description TEXT,
    current_status VARCHAR(20) NOT NULL DEFAULT 'UNKNOWN',

    CONSTRAINT chk_component_status
    CHECK (current_status IN (
        'NORMAL', 'WARNING', 'ERROR',
        'OFFLINE', 'UNKNOWN'
    ))
);

-- 2. Контролируемые параметры

CREATE TABLE parameters (
    id INTEGER GENERATED ALWAYS AS IDENTITY PRIMARY KEY,
    component_id INTEGER NOT NULL REFERENCES components(id),
    name VARCHAR(100) NOT NULL,
    unit VARCHAR(20),
    min_value DOUBLE PRECISION,
    max_value DOUBLE PRECISION,

    CONSTRAINT chk_parameter_range
    CHECK (
        min_value IS NULL
        OR max_value IS NULL
        OR min_value < max_value
    )
);

-- 3. История измерений

CREATE TABLE measurements (
    id BIGINT GENERATED ALWAYS AS IDENTITY PRIMARY KEY,
    parameter_id INTEGER NOT NULL REFERENCES parameters(id),
    timestamp TIMESTAMPTZ NOT NULL DEFAULT CURRENT_TIMESTAMP,
    value DOUBLE PRECISION NOT NULL,
    status_code INTEGER NOT NULL DEFAULT 0
);

-- 4. Диагностические события

CREATE TABLE diagnostic_events (
    id BIGINT GENERATED ALWAYS AS IDENTITY PRIMARY KEY,
    component_id INTEGER NOT NULL REFERENCES components(id),
    timestamp TIMESTAMPTZ NOT NULL DEFAULT CURRENT_TIMESTAMP,
    event_type VARCHAR(50) NOT NULL,
    description TEXT,
    severity VARCHAR(20) NOT NULL,

    CONSTRAINT chk_event_severity
    CHECK (severity IN ('INFO', 'WARNING', 'CRITICAL'))
);

-- 5. Правила диагностики

CREATE TABLE diagnostic_rules (
    id INTEGER GENERATED ALWAYS AS IDENTITY PRIMARY KEY,
    parameter_id INTEGER NOT NULL REFERENCES parameters(id),
    rule_name VARCHAR(100) NOT NULL,
    min_threshold DOUBLE PRECISION,
    max_threshold DOUBLE PRECISION,
    severity VARCHAR(20) NOT NULL,

    CONSTRAINT chk_rule_severity
    CHECK (severity IN ('WARNING', 'CRITICAL')),

    CONSTRAINT chk_rule_threshold
    CHECK (
        min_threshold IS NOT NULL
        OR max_threshold IS NOT NULL
    ),

    CONSTRAINT chk_rule_range
    CHECK (
        min_threshold IS NULL
        OR max_threshold IS NULL
        OR min_threshold < max_threshold
    )
);

-- Индексы

CREATE INDEX idx_measurements_parameter_time
ON measurements(parameter_id, timestamp);

CREATE INDEX idx_events_component_time
ON diagnostic_events(component_id, timestamp);
