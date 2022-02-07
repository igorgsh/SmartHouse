-- SEQUENCE: public.pv_minute_sequence

-- DROP SEQUENCE IF EXISTS public.pv_minute_sequence;

CREATE SEQUENCE IF NOT EXISTS public.pv_minute_sequence
    INCREMENT 1
    START 1
    MINVALUE 1
    MAXVALUE 9223372036854775807
    CACHE 1;

ALTER SEQUENCE public.pv_minute_sequence
    OWNER TO iobroker;