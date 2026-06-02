DROP TABLE IF EXISTS salary_log CASCADE;
DROP TABLE IF EXISTS appointments CASCADE;
DROP TABLE IF EXISTS salon_state CASCADE;
DROP TABLE IF EXISTS services CASCADE;
DROP TABLE IF EXISTS clients CASCADE;
DROP TABLE IF EXISTS employee_types CASCADE;
DROP TABLE IF EXISTS employees CASCADE;

CREATE TABLE employees (
    id              SERIAL PRIMARY KEY,
    full_name       VARCHAR(100) NOT NULL,
    specialization  VARCHAR(30) NOT NULL CHECK (specialization IN ('hairdresser', 'manicurist', 'cosmetologist', 'makeupartist')),
    experience      INT NOT NULL CHECK (experience >= 0 AND experience <= 50),
    rating          NUMERIC(2,1) NOT NULL CHECK (rating >= 0.0 AND rating <= 5.0),
    salary_base     NUMERIC(10,2) NOT NULL CHECK (salary_base >= 0),
    salary_percent  NUMERIC(5,2) NOT NULL CHECK (salary_percent >= 0 AND salary_percent <= 100),
    status          VARCHAR(20) NOT NULL DEFAULT 'active' CHECK (status IN ('active', 'vacation', 'sick_leave', 'inactive')),
    is_deleted      BOOLEAN NOT NULL DEFAULT FALSE
);

CREATE TABLE employee_types (
    id                    SERIAL PRIMARY KEY,
    employee_id           INT NOT NULL UNIQUE REFERENCES employees(id) ON DELETE CASCADE,
    type                  VARCHAR(30) NOT NULL CHECK (type IN ('hairdresser', 'manicurist', 'cosmetologist', 'makeupartist')),
    max_clients_per_day   INT NOT NULL CHECK (max_clients_per_day > 0),
    avg_service_time      INT NOT NULL CHECK (avg_service_time >= 15 AND avg_service_time <= 180)
);

CREATE TABLE clients (
    id                SERIAL PRIMARY KEY,
    full_name         VARCHAR(100) NOT NULL,
    phone             VARCHAR(25) NOT NULL UNIQUE,
    first_visit_date  DATE NOT NULL,
    total_spent       NUMERIC(12,2) NOT NULL DEFAULT 0 CHECK (total_spent >= 0),
    visits_count      INT NOT NULL DEFAULT 0 CHECK (visits_count >= 0),
    bonus_points      INT NOT NULL DEFAULT 0 CHECK (bonus_points >= 0),
    is_deleted        BOOLEAN NOT NULL DEFAULT FALSE
);

CREATE TABLE services (
    id                       SERIAL PRIMARY KEY,
    name                     VARCHAR(100) NOT NULL,
    duration_minutes         INT NOT NULL CHECK (duration_minutes >= 15 AND duration_minutes <= 180),
    price                    NUMERIC(10,2) NOT NULL CHECK (price >= 0),
    required_specialization  VARCHAR(30) NOT NULL CHECK (required_specialization IN ('hairdresser', 'manicurist', 'cosmetologist', 'makeupartist')),
    material_cost            NUMERIC(10,2) NOT NULL DEFAULT 0 CHECK (material_cost >= 0)
);

CREATE TABLE appointments (
    id                 SERIAL PRIMARY KEY,
    client_id          INT NOT NULL REFERENCES clients(id),
    employee_id        INT NOT NULL REFERENCES employees(id),
    service_id         INT NOT NULL REFERENCES services(id),
    appointment_date   DATE NOT NULL,
    appointment_time   TIME NOT NULL,
    status             VARCHAR(20) NOT NULL DEFAULT 'scheduled' CHECK (status IN ('scheduled', 'completed', 'cancelled')),
    price_at_booking   NUMERIC(10,2) NOT NULL CHECK (price_at_booking >= 0)
);

CREATE TABLE salon_state (
    id                        SERIAL PRIMARY KEY,
    current_salon_date        DATE NOT NULL,
    cash_balance              NUMERIC(12,2) NOT NULL CHECK (cash_balance >= 0),
    last_month_salary_paid    DATE,
    total_revenue             NUMERIC(12,2) NOT NULL DEFAULT 0 CHECK (total_revenue >= 0),
    total_expenses            NUMERIC(12,2) NOT NULL DEFAULT 0 CHECK (total_expenses >= 0)
);

CREATE TABLE salary_log (
    id                   SERIAL PRIMARY KEY,
    employee_id          INT NOT NULL REFERENCES employees(id),
    month_year           DATE NOT NULL,
    total_services_sum   NUMERIC(12,2) NOT NULL DEFAULT 0 CHECK (total_services_sum >= 0),
    salary_calculated    NUMERIC(12,2) NOT NULL CHECK (salary_calculated >= 0),
    paid_status          BOOLEAN NOT NULL DEFAULT FALSE,
    UNIQUE(employee_id, month_year)
);