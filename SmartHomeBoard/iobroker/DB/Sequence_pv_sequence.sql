-- SEQUENCE: public.pv_sequence

-- DROP SEQUENCE IF EXISTS public.pv_sequence;

CREATE SEQUENCE IF NOT EXISTS public.pv_sequence
    INCREMENT 1
    START 1
    MINVALUE 1
    MAXVALUE 9223372036854775807
    CACHE 1;

ALTER SEQUENCE public.pv_sequence
    OWNER TO iobroker;