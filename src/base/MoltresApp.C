#include "MoltresApp.h"
#include "Moose.h"
#include "AppFactory.h"
#include "ModulesApp.h"
#include "SquirrelApp.h"
#include "MooseSyntax.h"

// Kernels
#include "DelayedNeutronSource.h"
#include "ScalarTransportTimeDerivative.h"
#include "ScalarAdvectionArtDiff.h"
#include "PrecursorDecay.h"
#include "NtTimeDerivative.h"
#include "PrecursorSource.h"
#include "FissionHeatSource.h"
#include "TransientFissionHeatSource.h"
#include "INSMomentumKEpsilon.h"
#include "INSBoussinesqBodyForce.h"
#include "SigmaR.h"
#include "CoupledFissionEigenKernel.h"
#include "CoupledFissionKernel.h"
#include "SelfFissionEigenKernel.h"
#include "InScatter.h"
#include "GroupDiffusion.h"
#include "CoupledScalarAdvection.h"
#include "DivFreeCoupledScalarAdvection.h"
#include "GammaHeatSource.h"

// Boundary conditions
#include "LinLogPenaltyDirichletBC.h"
#include "VacuumConcBC.h"
#include "ConservativeAdvectionNoBCBC.h"
#include "DiffusionNoBCBC.h"
#include "ScalarAdvectionArtDiffNoBCBC.h"
#include "INSOutflowBC.h"
#include "INSSymmetryAxisBC.h"
#include "MatDiffusionFluxBC.h"
#include "CoupledScalarAdvectionNoBCBC.h"

// Materials
#include "GenericMoltresMaterial.h"
#include "CammiFuel.h"
#include "CammiModerator.h"
#include "Nusselt.h"
#include "GraphiteTwoGrpXSFunctionMaterial.h"
#include "MsreFuelTwoGrpXSFunctionMaterial.h"
#include "RoddedMaterial.h"

// Postprocessors
#include "LimitK.h"
#include "AverageFissionHeat.h"
#include "ElementL2Diff.h"
#include "DivisionPostprocessor.h"
#include "IntegralOldVariablePostprocessor.h"
#include "IntegralNewVariablePostprocessor.h"
#include "ElmIntegTotFissPostprocessor.h"
#include "ElmIntegTotFissHeatPostprocessor.h"
#include "ElmIntegTotFissNtsPostprocessor.h"

// AuxKernels
#include "FissionHeatSourceAux.h"
#include "FissionHeatSourceTransientAux.h"
#include "ModeratorHeatSourceTransientAux.h"
#include "MatDiffusionAux.h"
#include "ConstantDifferenceAux.h"

// Actions
#include "PrecursorAction.h"
#include "NtAction.h"

// DiracKernels
#include "DiracHX.h"

template <>
InputParameters
validParams<MoltresApp>()
{
  InputParameters params = validParams<MooseApp>();

  params.set<bool>("use_legacy_uo_initialization") = false;
  params.set<bool>("use_legacy_uo_aux_computation") = false;
  params.set<bool>("use_legacy_output_syntax") = false;

  return params;
}

MoltresApp::MoltresApp(InputParameters parameters) : MooseApp(parameters)
{
  Moose::registerObjects(_factory);
  ModulesApp::registerObjects(_factory);
  SquirrelApp::registerObjects(_factory);
  MoltresApp::registerObjects(_factory);

  Moose::associateSyntax(_syntax, _action_factory);
  ModulesApp::associateSyntax(_syntax, _action_factory);
  SquirrelApp::associateSyntax(_syntax, _action_factory);
  MoltresApp::associateSyntax(_syntax, _action_factory);
}

MoltresApp::~MoltresApp() {}

// External entry point for dynamic application loading
extern "C" void
MoltresApp__registerApps()
{
  MoltresApp::registerApps();
}
void
MoltresApp::registerApps()
{
  registerApp(MoltresApp);
}

// External entry point for dynamic object registration
extern "C" void
MoltresApp__registerObjects(Factory & factory)
{
  MoltresApp::registerObjects(factory);
}
void
MoltresApp::registerObjects(Factory & factory)
{
  registerKernel(DelayedNeutronSource);
  registerKernel(SigmaR);
  registerKernel(GammaHeatSource);
  registerKernel(ScalarTransportTimeDerivative);
  registerKernel(PrecursorSource);
  registerKernel(PrecursorDecay);
  registerKernel(CoupledScalarAdvection);
  registerKernel(DivFreeCoupledScalarAdvection);
  registerKernel(NtTimeDerivative);
  registerKernel(FissionHeatSource);
  registerKernel(TransientFissionHeatSource);
  registerKernel(InScatter);
  registerKernel(CoupledFissionEigenKernel);
  registerKernel(ScalarAdvectionArtDiff);
  registerKernel(CoupledFissionKernel);
  registerKernel(SelfFissionEigenKernel);
  registerKernel(INSMomentumKEpsilon);
  registerKernel(INSBoussinesqBodyForce);
  registerKernel(GroupDiffusion);
  registerBoundaryCondition(LinLogPenaltyDirichletBC);
  registerBoundaryCondition(ScalarAdvectionArtDiffNoBCBC);
  registerBoundaryCondition(VacuumConcBC);
  registerBoundaryCondition(ConservativeAdvectionNoBCBC);
  registerBoundaryCondition(DiffusionNoBCBC);
  registerBoundaryCondition(CoupledScalarAdvectionNoBCBC);
  registerBoundaryCondition(INSOutflowBC);
  registerBoundaryCondition(INSSymmetryAxisBC);
  registerBoundaryCondition(MatDiffusionFluxBC);
  registerMaterial(GenericMoltresMaterial);
  registerMaterial(CammiFuel);
  registerMaterial(CammiModerator);
  registerMaterial(Nusselt);
  registerMaterial(GraphiteTwoGrpXSFunctionMaterial);
  registerMaterial(MsreFuelTwoGrpXSFunctionMaterial);
  registerMaterial(RoddedMaterial);
  registerPostprocessor(LimitK);
  registerPostprocessor(IntegralOldVariablePostprocessor);
  registerPostprocessor(ElementL2Diff);
  registerPostprocessor(IntegralNewVariablePostprocessor);
  registerPostprocessor(DivisionPostprocessor);
  registerPostprocessor(ElmIntegTotFissPostprocessor);
  registerPostprocessor(ElmIntegTotFissHeatPostprocessor);
  registerPostprocessor(ElmIntegTotFissNtsPostprocessor);
  registerPostprocessor(AverageFissionHeat);
  registerAux(FissionHeatSourceAux);
  registerAux(FissionHeatSourceTransientAux);
  registerAux(ModeratorHeatSourceTransientAux);
  registerAux(MatDiffusionAux);
  registerAux(ConstantDifferenceAux);
  registerDiracKernel(DiracHX);
}

// External entry point for dynamic syntax association
extern "C" void
MoltresApp__associateSyntax(Syntax & syntax, ActionFactory & action_factory)
{
  MoltresApp::associateSyntax(syntax, action_factory);
}
void
MoltresApp::associateSyntax(Syntax & syntax, ActionFactory & action_factory)
{
  syntax.registerActionSyntax("PrecursorAction", "Precursors/*");
  syntax.registerActionSyntax("NtAction", "Nt");

  registerAction(PrecursorAction, "add_kernel");
  registerAction(PrecursorAction, "add_postprocessor");
  registerAction(PrecursorAction, "add_bc");
  registerAction(PrecursorAction, "add_variable");
  registerAction(PrecursorAction, "add_ic");
  registerAction(PrecursorAction, "add_dg_kernel");
  registerAction(PrecursorAction, "add_transfer");
  registerAction(PrecursorAction, "check_copy_nodal_vars");
  registerAction(PrecursorAction, "copy_nodal_vars");
  registerAction(NtAction, "add_kernel");
  registerAction(NtAction, "add_bc");
  registerAction(NtAction, "add_variable");
  registerAction(NtAction, "add_ic");
  registerAction(NtAction, "add_aux_variable");
  registerAction(NtAction, "add_aux_kernel");
  registerAction(NtAction, "check_copy_nodal_vars");
  registerAction(NtAction, "copy_nodal_vars");
}
