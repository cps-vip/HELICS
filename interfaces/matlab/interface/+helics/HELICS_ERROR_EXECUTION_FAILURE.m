function v = HELICS_ERROR_EXECUTION_FAILURE()
  persistent vInitialized;
  if isempty(vInitialized)
    vInitialized = helicsMEX(0, 63);
  end
  v = vInitialized;
end