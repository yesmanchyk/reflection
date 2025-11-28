nstructs = 4
nfields  = 8

last = 0
serializes = {}
for i=0,nstructs do
  struct_header = string.format("struct S%d {", i)
  lines = {struct_header}
  fields = {}
  values = {}
  kvs = {}
  for j=0,nfields do
    lines[#lines+1] = string.format("  int f%d;", j)
    kvs[#kvs+1] = string.format("f%d: %d ", j, j+1)
    fields[#fields+1] = string.format("f%d", j)
    values[#values+1] = tostring(1+j)
  end
  lines[#lines+1] = "};"

  lines[#lines+1] = string.format(
    "BOOST_HANA_ADAPT_STRUCT(S%d, %s);",
    i, table.concat(fields, ", ")
  )
  lines[#lines+1] = string.format(
    "S%d s%d{%s};", i, i, table.concat(values, ",")
  )
  table.insert(lines, "")
  serializes[#serializes+1] = string.format(
    "serialize(s%d);", i
  )
  last = 3+i*#lines
  vim.api.nvim_buf_set_lines(0, last, last, true, lines)
  last = last + #lines
end

lines = { string.format(
  "void serializeAll() { %s }", table.concat(serializes, " ")
) }
vim.api.nvim_buf_set_lines(0, last, last, true, lines)
