#!/usr/bin/env python
# -*- coding: utf-8 -*-

import logging
import Artus.Utility.logger as logger
log = logging.getLogger(__name__)

import argparse
import copy
import os
import re

import CombineHarvester.CombineTools.ch as ch

import Artus.Utility.tools as tools
import Artus.HarryPlotter.utility.plotconfigs as plotconfigs

import HiggsAnalysis.KITHiggsToTauTau.plotting.higgsplot as higgsplot
import HiggsAnalysis.KITHiggsToTauTau.plotting.configs.samples_run2 as samples
import HiggsAnalysis.KITHiggsToTauTau.plotting.configs.binnings as binnings
import HiggsAnalysis.KITHiggsToTauTau.plotting.configs.systematics_run2 as systematics

samples_dict = {
        'et' : [['nominal',['ztt','zll','zl','zj','ttj','vv','wj','qcd','ggh','bbh']]],
        'mt' : [['nominal',['ztt','zll','zl','zj','ttj','vv','wj','qcd','ggh','bbh']]],
        'em' : [['nominal',['ztt','zll','zl','zj','ttj','vv','wj','qcd','ggh','bbh']]],
        'tt' : [['nominal',['ztt','zll','zl','zj','ttj','vv','wj','qcd','ggh','bbh']]]
        }
mapping_process2sample = {
	"data_obs" : "data",
	"ZTT" : "ztt",
	"ZLL" : "zll",
	"ZL" : "zl",
	"ZJ" : "zj",
	"TT" : "ttj",
	"VV" : "vv",
	"W" : "wj",
	"QCD" : "qcd",
	"ggH" : "ggh",
	"bbH" : "bbh",
	"qqH" : "qqh",
	"VH" : "vh",
	"WH" : "wh",
	"ZH" : "zh",
}

def sample2process(sample):
        tmp_sample = re.match("(?P<sample>[^0-9]*).*", sample).groupdict().get("sample", "")
        return sample.replace(tmp_sample, dict([reversed(item) for item in mapping_process2sample.iteritems()]).get(tmp_sample, tmp_sample))


def _call_command(command):
	log.debug(command)
	logger.subprocessCall(command, shell=True)


if __name__ == "__main__":

	parser = argparse.ArgumentParser(description="Create ROOT inputs and datacards for SM HTT analysis.",
	                                 parents=[logger.loggingParser])

	parser.add_argument("-i", "--input-dir", required=True,
	                    help="Input directory.")
	parser.add_argument("-c", "--channel", action="append",
	                    default=[],
	                    help="Channel. This agument can be set multiple times. [Default: %(default)s]")
	parser.add_argument("--categories", action="append", nargs="+",
                	    default=[],
	                    help="Categories per channel. This agument needs to be set as often as --channels. [Default: %(default)s]")
	parser.add_argument("-m", "--higgs-masses", nargs="+", default=["125"],
	                    help="Higgs masses. [Default: %(default)s]")
	parser.add_argument("-s", "--samples", nargs="+", default=[],
	                    help="Samples used. [Default: %(default)s]")
	parser.add_argument("-x", "--quantity", default="0",
	                    help="Quantity. [Default: %(default)s]")
	parser.add_argument("--lumi", type=float, default=2.301,
	                    help="Luminosity for the given data in fb^(-1). [Default: %(default)s]")
	parser.add_argument("--for-dcsync", action="store_true", default=False,
	                    help="Produces simplified datacards for the synchronization exercise. [Default: %(default)s]")
	parser.add_argument("-w", "--weight", default="1.0",
	                    help="Additional weight (cut) expression. [Default: %(default)s]")
	parser.add_argument("--analysis-modules", default=[], nargs="+",
	                    help="Additional analysis Modules. [Default: %(default)s]")
	parser.add_argument("-a", "--args", default="",
	                    help="Additional Arguments for HarryPlotter. [Default: %(default)s]")
	parser.add_argument("-n", "--n-processes", type=int, default=1,
	                    help="Number of (parallel) processes. [Default: %(default)s]")
	parser.add_argument("-f", "--n-plots", type=int, nargs=2, default=[None, None],
	                    help="Number of plots for datacard inputs (1st arg) and for postfit plots (2nd arg). [Default: all]")
	parser.add_argument("-o", "--output-dir",
	                    default="$CMSSW_BASE/src/plots/htt_datacards/",
	                    help="Output directory. [Default: %(default)s]")
	parser.add_argument("-p", "--postfix",
	                    default="",
	                    help="Postfix for the datacard root files. [Default: %(default)s]")
	parser.add_argument("--clear-output-dir", action="store_true", default=False,
	                    help="Delete/clear output directory before running this script. [Default: %(default)s]")
	
	args = parser.parse_args()
	logger.initLogger(args)
	
	args.output_dir = os.path.abspath(os.path.expandvars(args.output_dir))
	if args.clear_output_dir and os.path.exists(args.output_dir):
		logger.subprocessCall("rm -r " + args.output_dir, shell=True)
	
	# initialisations for plotting
	sample_settings = samples.Samples()
	binnings_settings = binnings.BinningsDict()
	systematics_factory = systematics.SystematicsFactory()
	
	plot_configs = []
	output_files = []
	hadd_commands = []
	
	# initialise datacards
	tmp_input_root_filename_template = "input/${ANALYSIS}_${CHANNEL}_${BIN}_${SYSTEMATIC}_${ERA}.root"
	input_root_filename_template = "input/${ANALYSIS}_${CHANNEL}_${BIN}_${ERA}.root"
	bkg_histogram_name_template = "${BIN}/${PROCESS}"
	sig_histogram_name_template = "${BIN}/${PROCESS}${MASS}"
	bkg_syst_histogram_name_template = "${BIN}/${PROCESS}_${SYSTEMATIC}"
	sig_syst_histogram_name_template = "${BIN}/${PROCESS}${MASS}_${SYSTEMATIC}"
	output_root_filename_template = "datacards/common/${ANALYSIS}.input_${ERA}.root"
	if args.for_dcsync:
		output_root_filename_template = "datacards/common/${ANALYSIS}.inputs-sm-${ERA}-mvis.root"
	
	# args.categories = (args.categories * len(args.channel))[:len(args.channel)]
	print args.channel
	print args.categories
	for index, (channel, categories) in enumerate(zip(args.channel, args.categories)):
		tmp_output_files = []
		output_file = os.path.join(args.output_dir, "htt_%s.inputs-mssm-13TeV%s.root"%(channel,args.postfix))
		output_files.append(output_file)
		
		for category in categories:
			
			exclude_cuts = []
			if args.for_dcsync:
				if category[3:] == 'inclusive':
					exclude_cuts=["mt", "pzeta"]
				elif category[3:] == 'inclusivenotwoprong':
					exclude_cuts=["pzeta"]
			if "wjetscr" in category:
			    exclude_cuts.append("mt")
			if "qcdcr" in category or "_ss" in category:
			    exclude_cuts.append("os")
			
			for shape_systematic, list_of_samples in samples_dict[channel]:
				nominal = (shape_systematic == "nominal")
				list_of_samples = (["data"] if nominal else []) + list_of_samples
                                print args.samples
                                print list_of_samples
                                if args.samples:
                                    list_of_samples = args.samples
				
				for shift_up in ([True] if nominal else [True, False]):
					systematic = "nominal" if nominal else (shape_systematic + ("Up" if shift_up else "Down"))
					
					log.debug("Create inputs for (samples, systematic) = ([\"{samples}\"], {systematic}), (channel, category) = ({channel}, {category}).".format(
							samples="\", \"".join(list_of_samples),
							channel=channel,
							category=category,
							systematic=systematic
					))
					
					# prepare plotting configs for retrieving the input histograms
					config = sample_settings.get_config(
							samples=[getattr(samples.Samples, sample) for sample in list_of_samples],
							channel=channel,
							category="catHttMSSM13TeV_"+category,
							weight=args.weight,
							lumi = args.lumi * 1000,
							exclude_cuts=exclude_cuts,
							higgs_masses=args.higgs_masses,
							mssm=True
					)
					
					systematics_settings = systematics_factory.get(shape_systematic)(config)
					# TODO: evaluate shift from datacards_per_channel_category.cb
					config = systematics_settings.get_config(shift=(0.0 if nominal else (1.0 if shift_up else -1.0)))
					
					config["x_expressions"] = [args.quantity]
					
					binnings_key = "binningHttMSSM13TeV_"+category+"_svfitMass"
					if binnings_key in binnings_settings.binnings_dict:
						config["x_bins"] = [binnings_key]
					else:
						config["x_bins"] = ["35,0.0,350.0"]
					
					config["directories"] = [args.input_dir]
					
					histogram_name_template = bkg_histogram_name_template if nominal else bkg_syst_histogram_name_template
					config["labels"] = [histogram_name_template.replace("$", "").format(
							PROCESS=sample2process(sample),
							BIN=category,
							SYSTEMATIC=systematic
					) for sample in config["labels"]]
					
					tmp_output_file = os.path.join(args.output_dir, tmp_input_root_filename_template.replace("$", "").format(
							ANALYSIS="htt",
							CHANNEL=channel,
							BIN=category,
							SYSTEMATIC=systematic,
							ERA="13TeV"
					))
					tmp_output_files.append(tmp_output_file)
					config["output_dir"] = os.path.dirname(tmp_output_file)
					config["filename"] = os.path.splitext(os.path.basename(tmp_output_file))[0]
				
					config["plot_modules"] = ["ExportRoot"]
					config["file_mode"] = "UPDATE"
			
					if "legend_markers" in config:
						config.pop("legend_markers")
					if args.for_dcsync:
					    config["wjets_from_mc"] = [True,True]
			
					plot_configs.append(config)
			
		hadd_commands.append("hadd -f {DST} {SRC} && rm {SRC}".format(
				DST=output_file,
				SRC=" ".join(tmp_output_files)
		))
	
	#if log.isEnabledFor(logging.DEBUG):
	#	import pprint
	#	pprint.pprint(plot_configs)
	
	# delete existing output files
	tmp_output_files = list(set([os.path.join(config["output_dir"], config["filename"]+".root") for config in plot_configs[:args.n_plots[0]]]))
	for output_file in tmp_output_files:
		if os.path.exists(output_file):
			os.remove(output_file)
			log.debug("Removed file \""+output_file+"\" before it is recreated again.")
	output_files = list(set(output_files))
	
	# create input histograms with HarryPlotter
	higgsplot.HiggsPlotter(list_of_config_dicts=plot_configs, list_of_args_strings=[args.args], n_processes=args.n_processes, n_plots=args.n_plots[0])
	tools.parallelize(_call_command, hadd_commands, n_processes=args.n_processes)
	
	debug_plot_configs = []
	for output_file in output_files:
		debug_plot_configs.extend(plotconfigs.PlotConfigs().all_histograms(output_file, plot_config_template={"markers":["E"], "colors":["#FF0000"]}))
	higgsplot.HiggsPlotter(list_of_config_dicts=debug_plot_configs, list_of_args_strings=[args.args], n_processes=args.n_processes, n_plots=args.n_plots[0])