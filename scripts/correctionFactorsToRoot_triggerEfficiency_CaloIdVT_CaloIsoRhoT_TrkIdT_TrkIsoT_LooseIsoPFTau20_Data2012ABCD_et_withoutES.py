#!/usr/bin/env python
# -*- coding: utf-8 -*-

import logging
import Artus.Utility.logger as logger
log = logging.getLogger(__name__)

import argparse
import os

import ROOT
ROOT.PyConfig.IgnoreCommandLineOptions = True
ROOT.gROOT.SetBatch(True)

import HiggsAnalysis.KITHiggsToTauTau.triggerTurnOnParametrisation as triggerTurnOnParametrisation


if __name__ == "__main__":

	parser = argparse.ArgumentParser(description="Store correction factors in ROOT histograms. Here: trigger efficiencies (Tau Trigger for ET channel (without ES correction), Data2012ABCD)",
	                                 parents=[logger.loggingParser])

	parser.add_argument("-n", "--histogram-name", default="triggerEfficieny",
	                    help="Histogram name. [Default: %(default)s]")
	parser.add_argument("-o", "--output",
	                    default="$CMSSW_BASE/src/HiggsAnalysis/KITHiggsToTauTau/data/root/triggerEfficiency_CaloIdVT_CaloIsoRhoT_TrkIdT_TrkIsoT_LooseIsoPFTau20_Data2012ABCD_et_withoutES.root",
	                    help="Output ROOT file. [Default: %(default)s]")
	
	parser.add_argument("--n-bins-pt", type=int, default=500,
	                    help="Number of pt bins. [Default: %(default)s]")
	parser.add_argument("--min-pt", type=float, default=0.0,
	                    help="Minium pt. [Default: %(default)s]")
	parser.add_argument("--max-pt", type=float, default=500.0,
	                    help="Maxium pt. [Default: %(default)s]")

	args = parser.parse_args()
	logger.initLogger(args)

	eta_border_eb_ee = 1.5
	eta_bins_with_parameters = [
		{
			"low" : -10.0,
			"high" : -eta_border_eb_ee,
			"parameters" : [18.701715, 0.216523, 0.148111, 2.245081, 0.895320],
		},
		{
			"low" : -eta_border_eb_ee,
			"high" : eta_border_eb_ee,
			"parameters" : [18.604910, 0.276042, 0.137039, 2.698437, 0.940721],
		},
		{
			"low" : eta_border_eb_ee,
			"high" : 10.0,
			"parameters" : [18.701715, 0.216523, 0.148111, 2.245081, 0.895320],
		},
	]
	
	args.output = os.path.expandvars(args.output)
	dirname = os.path.dirname(args.output)
	if dirname != "" and not os.path.exists(dirname):
		os.makedirs(dirname)
	
	root_file = ROOT.TFile(args.output, "RECREATE")
	histogram = triggerTurnOnParametrisation.fill_root_histogram(args.n_bins_pt, args.min_pt, args.max_pt,
	                                                             eta_bins_with_parameters, args.histogram_name)
	root_file.Write()
	root_file.Close()
	log.info("Correction factors have been stored in histogram \"%s/%s\"." % (args.output, args.histogram_name))
