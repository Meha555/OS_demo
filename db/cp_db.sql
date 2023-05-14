
CREATE TABLE "message" (
  "m_id" integer NOT NULL,
  "t_id" integer,
  "b_id" integer,
  "data_id" integer,
  "op_type" text,
  "data" text,
  PRIMARY KEY ("m_id")
);

CREATE TABLE "config" (
  "c_id" integer NOT NULL,
  "buffer1_size" integer,
  "buffer2_size" integer,
  "buffer3_size" integer,
  "put_num" integer,
  "move_num" integer,
  "get_num" integer,
  "put_speed" integer,
  "move_speed" integer,
  "get_speed" integer,
  PRIMARY KEY ("c_id")
);

CREATE TABLE "result" (
  "r_id" integer,
  "run_time" text,
  "cur_num" integer,
  "putin_num" integer,
  "getout_num" integer,
  "avg_num" integer
  PRIMARY KEY ("r_id")
);

