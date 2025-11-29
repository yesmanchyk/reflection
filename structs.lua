nstructs = 80 
nfields  = 40

last = 0
serializes = {}
for i=0,nstructs do
  struct_header = string.format("struct S%d {", i)
  lines = {struct_header}
  values = {}
  kvs = {}
  for j=0,nfields do
    lines[#lines+1] = string.format("  int f%d;", j)
    xs = string.rep("X", j+1)
    kvs[#kvs+1] = string.format("f%d=%s", j, xs)
    values[#values+1] = tostring((1+j)*10)
  end
  lines[#lines+1] = string.format(
    "} s%d{%s};", i, table.concat(values, ",")
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
