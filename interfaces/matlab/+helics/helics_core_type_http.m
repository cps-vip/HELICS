function v = helics_core_type_http()
  persistent vInitialized;
  if isempty(vInitialized)
    vInitialized = helicsMEX(0, 1464812633);
  end
  v = vInitialized;
end
