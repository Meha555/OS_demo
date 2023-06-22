CREATE TABLE sqlite_sequence(name,seq);
CREATE TABLE IF NOT EXISTS "config" (
  "c_id" integer NOT NULL PRIMARY KEY AUTOINCREMENT,
  "buffer1_size" integer,
  "buffer2_size" integer,
  "buffer3_size" integer,
  "put_num" integer,
  "move_num" integer,
  "get_num" integer,
  "put_speed" integer,
  "move_speed" integer,
  "get_speed" integer
);
CREATE TABLE IF NOT EXISTS "message" (
  "m_id" integer NOT NULL PRIMARY KEY AUTOINCREMENT,
  "t_id" integer,
  "b_id" integer,
  "op_type" text,
  "data" text
);
CREATE TABLE IF NOT EXISTS "result" (
  "r_id" integer NOT NULL PRIMARY KEY AUTOINCREMENT,
  "run_time" text,
  "cur_num" integer,
  "putin_num" integer,
  "getout_num" integer,
  "putin_buf1" integer,
  "putin_buf2" integer,
  "putin_buf3" integer,
  "getout_buf1" integer,
  "getout_buf2" integer,
  "getout_buf3" integer,
  "avg_num" integer
);