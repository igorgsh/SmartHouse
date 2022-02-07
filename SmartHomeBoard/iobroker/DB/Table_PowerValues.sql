-- Table: public.PowerValues

-- DROP TABLE IF EXISTS public."PowerValues";

CREATE TABLE IF NOT EXISTS public."PowerValues"
(
    id bigint NOT NULL,
    ts timestamp with time zone,
    section text COLLATE pg_catalog."default",
    day_night boolean,
    voltage_a real,
    voltage_b real,
    voltage_c real,
    current_a real,
    current_b real,
    current_c real,
    frequency_a real,
    frequency_b real,
    frequency_c real,
    power_a real,
    power_b real,
    power_c real,
    energy_a real,
    energy_b real,
    energy_c real,
    powerfactor_a real,
    powerfactor_b real,
    powerfactor_c real,
    consumption_a real,
    consumption_b real,
    consumption_c real,
    CONSTRAINT "PowerValues_pkey" PRIMARY KEY (id)
)

TABLESPACE pg_default;

ALTER TABLE IF EXISTS public."PowerValues"
    OWNER to iobroker;
-- Index: PV_ts_sect_ndx

-- DROP INDEX IF EXISTS public."PV_ts_sect_ndx";

CREATE INDEX IF NOT EXISTS "PV_ts_sect_ndx"
    ON public."PowerValues" USING btree
    (ts ASC NULLS LAST, section COLLATE pg_catalog."default" ASC NULLS LAST)
    TABLESPACE pg_default;