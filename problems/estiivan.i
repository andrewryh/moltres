[Mesh]
  file = 'test.msh'
[]

[Variables]
  [./diffused]
    order = FIRST
    family = LAGRANGE
  [../]
[]

[Kernels]
  [./diff]
    type = Diffusion
    variable = diffused
  [../]
[]

[BCs]
  [./top]
    type = DirichletBC
    variable = diffused
    boundary = 'top'
    value = 1
  [../]

  [./left]
    type = DirichletBC
    variable = diffused
    boundary = 'left'
    value = 0
  [../]
[]

[Executioner]
  type = Steady
  solve_type = 'PJFNK'
[]

[Outputs]
  execute_on = 'timestep_end'
  exodus = true
[]
