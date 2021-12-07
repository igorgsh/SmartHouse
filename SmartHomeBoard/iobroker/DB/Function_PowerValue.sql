-- FUNCTION: public.PowerValue()

-- DROP FUNCTION IF EXISTS public."PowerValue"();

CREATE OR REPLACE FUNCTION public."PowerValue"()
    RETURNS trigger
    LANGUAGE 'plpgsql'
    COST 100
    VOLATILE NOT LEAKPROOF
AS $BODY$
BEGIN
	CALL "AddPowerValues"(NEW.id, NEW.ts, NEW.val);
	RETURN NULL;
END
$BODY$;

ALTER FUNCTION public."PowerValue"()
    OWNER TO iobroker;
