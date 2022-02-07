-- Table: public.pv_minute

-- DROP TABLE IF EXISTS public.pv_minute;

CREATE TABLE IF NOT EXISTS public.pv_minute
(
    id bigint NOT NULL,
    ts timestamp with time zone,
    section text COLLATE pg_catalog."default",
    day_night boolean,
    voltage_a_min real,
    voltage_b_min real,
    voltage_c_min real,
    voltage_a_max real,
    voltage_b_max real,
    voltage_c_max real,
    current_a_max real,
    current_b_max real,
    current_c_max real,
    frequency_a_min real,
    frequency_b_min real,
    frequency_c_min real,
    frequency_a_max real,
    frequency_b_max real,
    frequency_c_max real,
    power_a_max real,
    power_b_max real,
    power_c_max real,
    energy_a_day real,
    energy_b_day real,
    energy_c_day real,
    energy_a_night real,
    energy_b_night real,
    energy_c_night real,
    consumption_a_day real,
    consumption_b_day real,
    consumption_c_day real,
    powerfactor_a_min real,
    powerfactor_b_min real,
    powerfactor_c_min real,
    energy_a real,
    energy_b real,
    energy_c real,
    powerfactor_a_max real,
    powerfactor_b_max real,
    powerfactor_c_max real,
    consumption_a_night real,
    consumption_b_night real,
    consumption_c_night real,
    CONSTRAINT pv_minute_pkey PRIMARY KEY (id),
    CONSTRAINT pv_minute_ts_sect UNIQUE (ts, section)
)

TABLESPACE pg_default;

ALTER TABLE IF EXISTS public.pv_minute
    OWNER to iobroker;
-- Index: pv_minute_ts_sect_ndx

-- DROP INDEX IF EXISTS public.pv_minute_ts_sect_ndx;

CREATE INDEX IF NOT EXISTS pv_minute_ts_sect_ndx
    ON public.pv_minute USING btree
    (ts ASC NULLS LAST, section COLLATE pg_catalog."default" ASC NULLS LAST)
    TABLESPACE pg_default;