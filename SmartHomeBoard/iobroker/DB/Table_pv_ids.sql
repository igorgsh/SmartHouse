-- Table: public.pv_ids

-- DROP TABLE IF EXISTS public.pv_ids;

CREATE TABLE IF NOT EXISTS public.pv_ids
(
    id bigint NOT NULL,
    section character varying(100) COLLATE pg_catalog."default" NOT NULL,
    parameter character varying(100) COLLATE pg_catalog."default" NOT NULL,
    number_id bigint NOT NULL,
    CONSTRAINT pv_ids_pkey PRIMARY KEY (id),
    CONSTRAINT pv_ids_unique UNIQUE (number_id),
    CONSTRAINT sect_parms UNIQUE (section, parameter)
)

TABLESPACE pg_default;

ALTER TABLE IF EXISTS public.pv_ids
    OWNER to iobroker;
-- Index: pv_m_numberid_ndx

-- DROP INDEX IF EXISTS public.pv_m_numberid_ndx;

CREATE INDEX IF NOT EXISTS pv_m_numberid_ndx
    ON public.pv_ids USING btree
    (number_id ASC NULLS LAST)
    TABLESPACE pg_default;